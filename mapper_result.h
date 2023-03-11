#pragma once
#include "mapreduce.h"
#include <string>
namespace pglang {
namespace mapreduce {
/* MapperResult file format

    |MaxKeySize(32)|KeySize(32)|

    |Key1(MaxKeySize)|StartPosition(64)|
    |Key2(MaxKeySize)|StartPosition(64)|
    |Key3(MaxKeySize)|StartPosition(64)|
    ...
   //Key1's values
    |ValueSize(32)|Value(ValueSize)|
    ...





*/

class MapperResultReader {
  public:
    MapperResultReader(const std::string &_path, const MapperResult &result);

  private:
};
} // namespace mapreduce
} // namespace pglang
