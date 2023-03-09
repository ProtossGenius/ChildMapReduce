#pragma once
#include <string>

namespace pglang {
namespace mapreduce {

template <class Type> Type valueOf(const std::string &str) {
    return Type::valueOf(str);
}

#define REG_VAL_OF(type, func)                                                 \
    template <type> type valueOf(const std::string &str) { return func(str); }

REG_VAL_OF(int, std::stoi);

// REG_VAL_OF(double, std::stod);
REG_VAL_OF(long, std::stol);
// REG_VAL_OF(long double, std::stold);
REG_VAL_OF(unsigned long, std::stoul);
REG_VAL_OF(unsigned long long, std::stoull);
REG_VAL_OF(long long, std::stoll);
} // namespace mapreduce
} // namespace pglang
