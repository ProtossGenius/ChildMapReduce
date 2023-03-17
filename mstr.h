#pragma once
#include <functional>
#include <string>
namespace pglang {
namespace mapreduce {

void split(const std::string &str, const std::string &split,
           std::function<void(const std::string &)> cb);

} // namespace mapreduce
} // namespace pglang
