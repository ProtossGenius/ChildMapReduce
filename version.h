#pragma once

#ifndef VERSION
#define VERSION "1.0.0.0"
#endif

namespace pglang {
namespace mapreduce {
const char *GetVersion() { return VERSION; }

} // namespace mapreduce
} // namespace pglang
