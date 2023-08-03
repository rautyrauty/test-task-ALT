#include "lib.hpp"

nlohmann::ordered_json GetApiALT(const std::string& target)
{
    using namespace boost::beast;
    using namespace boost::asio;

    const std::string host = "rdb.altlinux.org";
    const std::string port = "443"; // ONLY HTTPS

    io_service svc;
    ssl::context ctx(ssl::context::sslv23_client);
    ssl::stream<ip::tcp::socket> ssocket(svc,ctx);

    ip::tcp::resolver resolver(svc);
    auto it = resolver.resolve(host, port);
    connect(ssocket.lowest_layer(), it);

    ssocket.handshake(ssl::stream_base::handshake_type::client);

    http::request<http::string_body> req{ http::verb::get, target, 11 };
    req.set(http::field::host, host);
    http::write(ssocket, req);
    http::response_parser<http::string_body> res;
    res.body_limit((std::numeric_limits<std::uint64_t>::max)());
    flat_buffer fbuffer;
    http::read(ssocket, fbuffer, res);
    if (res.get().result_int() != 200)
    {
        throw std::domain_error(res.get().body());
    }
    return nlohmann::ordered_json::parse(res.get().body());
}

int VersionReleaseComp(std::string v1, std::string v2)
{
    auto is_digit = [](const char c){ if (c>='0' and c <='9') return true; else return false;};
    int vnum1 = 0, vnum2 = 0;
    for (int i = 0, j = 0; (i < v1.length() or j < v2.length());)
    {
        while (i < v1.length() and not is_digit(v1[i]) ) {
            i++;
        }
        while (i < v1.length() and is_digit(v1[i]) ) {
            vnum1 = vnum1 * 10 + (v1[i] - '0');
            i++;
        }

        while (j < v2.length() and not is_digit(v2[j])) {
            j++;
        }
        while (j < v2.length() and is_digit(v2[j])) {
            vnum2 = vnum2 * 10 + (v2[j] - '0');
            j++;
        }

        if (vnum1 > vnum2) return  1;
        if (vnum2 > vnum1) return -1;

        vnum1 = vnum2 = 0;
        i++;
        j++;
    }
    return 0;
}

nlohmann::ordered_json GetBranchesAltDiff(const std::string& first_b,const std::string& second_b)
{
    using namespace nlohmann;

    ordered_json answer;
    auto push_package = [&answer](const std::string& arch, const char* s,const ordered_json& package) {
        answer[arch][s]["packages"].push_back(package);
        if (answer[arch][s]["length"].is_null()) answer[arch][s]["length"] = 0;
        int tmp = answer[arch][s]["length"];
        tmp += 1;
        answer[arch][s]["length"] = tmp;
    };

    auto NameComp = [](const auto& a,const auto& b){ return a["name"] < b["name"]; };

    ordered_json archs = GetApiALT("https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=" + first_b)["archs"];
    archs.merge_patch(GetApiALT(   "https://rdb.altlinux.org/api/site/all_pkgset_archs?branch=" + second_b)["archs"]);

    nlohmann::ordered_json first, second;
    for(auto arch_it = archs.begin(); arch_it != archs.end(); arch_it+=1)
    {
        std::cout << "Processing of the \"" + std::string((*arch_it)["arch"]) + "\" arch has started\n";

        if ((*arch_it)["arch"] == "srpm") // broken branch !?
        {
            std::cout << "srpm skipped\n";
            continue;
        }
        try
        {
            first  = GetApiALT(std::string("/api/export/branch_binary_packages/") + first_b  + "?arch=" + std::string((*arch_it)["arch"]));
            if(not std::is_sorted(first["packages"].begin(), first["packages"].end(), NameComp))
            {
                std::sort(first["packages"].begin(),first["packages"].end(), NameComp);
            }
            std::cout << "Branch " << first_b << ", arch " << (*arch_it)["arch"] << " loaded\n";

            second = GetApiALT(std::string("/api/export/branch_binary_packages/") + second_b + "?arch=" + std::string((*arch_it)["arch"]));
            if(not std::is_sorted(second["packages"].begin(), second["packages"].end(), NameComp))
            {
                std::sort(second["packages"].begin(), second["packages"].end(), NameComp);
            }
            std::cout << "Branch " << second_b << ", arch " << (*arch_it)["arch"] << " loaded\n";
        }
        catch (const std::exception& e)
        {
            std::cout << "Arch " << (*arch_it)["arch"] << " load error: " << '\n' << e.what()
                      << "Arch " << (*arch_it)["arch"] << " skipped\n";
            continue;
        }

        size_t i = 0, k = 0;
        while (i < first["length"] and k < second["length"])
        {
            if (first["packages"][i]["name"] < second["packages"][k]["name"])
            {
                push_package((*arch_it)["arch"], "only_in_first", first["packages"][i]);
                i += 1;
                continue;
            }
            else if (first["packages"][i]["name"] > second["packages"][k]["name"])
            {
                push_package((*arch_it)["arch"], "only_in_second", second["packages"][k]);
                k += 1;
                continue;
            }

            int res = VersionReleaseComp(first["packages"][i]["release"], second["packages"][k]["release"]);
            if (res > 0) push_package((*arch_it)["arch"], "in_first_release_greater", first["packages"][i]);
            else if ((res == 0) and (VersionReleaseComp(first["packages"][i]["version"], second["packages"][k]["version"])>0)) push_package((*arch_it)["arch"], "in_first_release_greater", first["packages"][i]);

            i+=1;
            k+=1;
        }
        while (i < first["length"])  push_package((*arch_it)["arch"], "only_in_first",  first["packages"][i++]);
        while (k < second["length"]) push_package((*arch_it)["arch"], "only_in_second", second["packages"][k++]);

    }

    return answer;
}
