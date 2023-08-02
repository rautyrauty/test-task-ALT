#include "lib.hpp"

nlohmann::json GetBranchALT(const std::string branch)
{
    using namespace boost::beast;
    using namespace boost::asio;

    const std::string host = "rdb.altlinux.org";
    const std::string target  = "/api/export/branch_binary_packages/" + branch;
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
    return nlohmann::json::parse(res.get().body());
}

nlohmann::json GetBranchesALTDiff(const std::string first_b,const std::string second_b)
{
    using namespace nlohmann;

    json first  = GetBranchALT(first_b);
    std::cout << "Branch " << first_b  << " loaded\n";

    json second = GetBranchALT(second_b);
    std::cout << "Branch " << second_b << " loaded\n";

    json answer = json::parse(R"(
                                  {
                                    "only_in_first":{
                                        "length": 0,
                                        "array": []
                                     },
                                    "only_in_second":{
                                        "length": 0,
                                        "array": []
                                     },
                                    "in_first_release_greater":{
                                        "length": 0,
                                        "array": []
                                     }
                                  }
                                 )");

    auto push_package = [&answer](const char* s,const json& package) {
        answer[s]["array"].push_back(package);
        int tmp = answer[s]["length"];
        tmp+=1;
        answer[s]["length"] = tmp;
    };

    size_t i = 0, k = 0;
    while (i < first["length"] and k < second["length"])
    {
        if (first["packages"][i]["name"] < second["packages"][k]["name"])
        {
            push_package("only_in_first", first["packages"][i]);
            i += 1;
        }
        else if (first["packages"][i]["name"] > second["packages"][k]["name"])
        {
            push_package("only_in_second", second["packages"][k]);
            k += 1;
        }
        else if (first["packages"][i]["release"] > second["packages"][k]["release"])
        {
            i+=1;
            k+=1;
            push_package("in_first_release_greater", first["packages"][i]);
        }
        else
        {
            i+=1;
            k+=1;
        }
    }
    while (i < first["length"])  push_package("only_in_first",  first["packages"][i++]);
    while (k < second["length"]) push_package("only_in_second", second["packages"][k++]);


    return answer;
}
