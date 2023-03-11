#include "defer.h"
#include "file_reader.h"
#include "file_spliter.h"
#include "nlohmann/json.hpp"
#include "param_parser.h"
#include <cstdio>
#include <fstream>
#include <iostream>
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
    auto        u(std::make_unique<LocalFileReader>("out.txt"));
    FileSpliter fs(std::move(u), 20);
    fs.split([](const auto &str) { cout << str << endl; });
}
