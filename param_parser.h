#pragma once
#include <map>
#include <string>
#include <vector>
namespace pglang {
namespace mapreduce {

class ParamParser {
  public:
    ParamParser(int arg, char const **args);
    ParamParser(ParamParser &&ant);

  public:
    std::string value(const std::string &param) const {
        auto iter = params.find(param);
        return iter != params.end() ? iter->second : "";
    }
    std::string value(const std::vector<std::string> &params) const;

    const std::vector<std::string> &normalPrarms() { return normal_params; }
    const std::map<std::string, std::string> &paramsMap() { return params; }

  private:
    std::map<std::string, std::string> params;
    std::vector<std::string>           normal_params;
};

class CmdLineRegist {
  public:
    CmdLineRegist(ParamParser &&parser)
        : param_parser(std::move(parser)) {}
    void title(const std::string &tt) { this->_title = tt; }
    void registe(const std::vector<std::string> &key, std::string &value,
                 const std::string &desc, const std::string &_default = "");
    void parse() const;
    void printInfo() const;

  private:
    ParamParser              param_parser;
    std::vector<std::string> registe_info;
    std::string              _title;
};

} // namespace mapreduce
} // namespace pglang
