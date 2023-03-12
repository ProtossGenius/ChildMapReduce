#pragma once

#include <fstream>
namespace pglang {
namespace mapreduce {

const size_t FSIZE_K = 1024, FSIZE_M = 1024 * FSIZE_K, FSIZE_G = 1024 * FSIZE_M,
             FSIZE_T = 1024 * FSIZE_G;

template <class type> union ftype {
    type value;
    char arr[ sizeof(type) ];
};

} // namespace mapreduce
} // namespace pglang
