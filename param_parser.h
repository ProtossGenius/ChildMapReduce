#pragma once
#include <map>
#include <string>
#include <vector>
namespace pglang {
namespace mapreduce {

class ParamParser {
  public:
    ParamParser(int arg, char const **args);
    const std::string value(const std::string &param) {
        return params[ param ];
    }

    const std::vector<std::string> &normalPrarms() { return normal_params; }
    const std::map<std::string, std::string> &paramsMap() { return params; }

  private:
    std::map<std::string, std::string> params;
    std::vector<std::string>           normal_params;
};

} // namespace mapreduce
} // namespace pglang
