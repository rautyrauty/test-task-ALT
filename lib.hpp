#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

#include "json.hpp"

nlohmann::json GetBranchALT(const std::string branch);
nlohmann::json GetBranchesALTDiff(const std::string first_b,const std::string second_b);
