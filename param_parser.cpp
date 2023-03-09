#include "param_parser.h"

namespace pglang {
namespace mapreduce {

// bin -a a -b b c c c
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

} // namespace mapreduce
} // namespace pglang
