#include "mstr.h"

namespace pglang {
namespace mapreduce {
void split(const std::string &str, const std::string &split,
           std::function<void(const std::string &)> cb) {
    int start, end = -1 * split.size();
    do {
        start = end + split.size();
        end   = str.find(split, start);
        cb(str.substr(start, end - start));
    } while (end != -1);
}
} // namespace mapreduce
} // namespace pglang
