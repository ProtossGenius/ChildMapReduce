#include "param_parser.h"
#include <fstream>
#include <string>
int main(int arg, const char **argv) {
    using namespace pglang::mapreduce;
    using namespace std;
    CmdLineRegist reg({arg, argv});
    string        size;
    string        file_name;
    reg.registe({"-s", "-size"}, size, "file size", "5000000");
    reg.registe({"-n", "-name"}, file_name, "file size", "out.txt");
    reg.parse();
    char r[ 60 ]{0};
    int  ptr = 0;
    for (int i = 0; i < 26; ++i)
        r[ ptr++ ] = 'a' + i;
    for (int i = 0; i < 10; ++i)
        r[ ptr++ ] = '0' + i;
    r[ ptr++ ] = ' ';
    r[ ptr++ ] = '\n';
    fstream fs(file_name, ios::out);
    size_t  sz = stoull(size);
    for (int i = 0; i < sz; ++i) {
        fs << r[ rand() % ptr ];
    }
    fs.flush();
    fs.close();
}
