#include "defer.h"
#include "file_reader.h"
#include "file_spliter.h"
#include "nlohmann/json.hpp"
#include "param_parser.h"
#include "thread_pool.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <leveldb/db.h>
#include <map>
#include <stack>
#include <string>
#include <vector>
using namespace std;
//
int main(int arg, const char **args) {
    using namespace pglang::mapreduce;
    CmdLineRegist reg({arg, args});
    reg.parse();
}
