#include "param_parser.h"
#include <iostream>
#include <sstream>
namespace pglang {
namespace mapreduce {

ParamParser::ParamParser(int arg, char const **args) {
    std::string key;
    for (int i = 0; i < arg; ++i) {
        const char *str = args[ i ];
        // key
        if (str[ 0 ] == '-') {
            key           = str;
            params[ key ] = "true"; // default value is true
            continue;
        }

        if (key == "") {
            normal_params.push_back(str);
            continue;
        }
        params[ key ] = str;
        key           = "";
    }
}

ParamParser::ParamParser(ParamParser &&ant) {
    params.swap(ant.params);
    normal_params.swap(ant.normal_params);
}

std::string ParamParser::value(const std::vector<std::string> &param) const {
    for (const auto &key : param) {
        std::string v = value(key);
        if (v != "") return v;
    }

    return "";
}

void CmdLineRegist::registe(const std::vector<std::string> &key,
                            std::string &value, const std::string &desc,
                            const std::string &_default) {
    auto v2 = param_parser.value(key);

    if (v2 != "") value = v2;
    if (value == "") value = _default;
    std::stringstream ss;
    for (int i = 0; i < key.size(); ++i) {
        if (i != 0) ss << ",";
        ss << key[ i ];
    }
    ss << "\n\tdesc:"
          "\n\t\t"
       << desc
       << "\n\tdefault value:"
          "\n\t\t"
       << _default;

    registe_info.push_back(ss.str());
}

void CmdLineRegist::parse() const {
    if (param_parser.value({"-h", "--h", "-help", "--help"}) == "") return;

    printInfo();
    exit(0);
}

void CmdLineRegist::printInfo() const {
    std::cout << _title << std::endl;
    for (const auto &info : registe_info) {
        std::cout << info << std::endl;
    }
}

} // namespace mapreduce
} // namespace pglang
