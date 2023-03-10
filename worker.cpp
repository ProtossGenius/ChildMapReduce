#include "param_parser.h"
#include <cstdio>
#include <iostream>
#include <stack>
#include <string>
#include <vector>
using namespace std;
// 你好
int main(int arg, const char **args) {
    using namespace pglang::mapreduce;
    CmdLineRegist reg({arg, args});
    string        hello;
    reg.registe({"-hello"}, hello, "hello world", "you're welcome");
    reg.parse();
    cout << hello << endl;
}
