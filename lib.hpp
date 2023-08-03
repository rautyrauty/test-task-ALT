#include <string>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>

#include "json.hpp"

nlohmann::ordered_json GetApiALT(const std::string& target);
nlohmann::ordered_json GetBranchesAltDiff(const std::string& first_b,const std::string& second_b);
int VersionReleaseComp(std::string v1, std::string v2);
