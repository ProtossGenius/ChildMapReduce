#pragma once
#include "file_reader.h"
#include "mapreduce.h"
#include <fstream>
#include <memory>
#include <string>
namespace pglang {
namespace mapreduce {
/* MapperResult file format

    |MaxKeySize(32)|KeyNum(32)|

    |Key1(MaxKeySize)|StartPosition(64)|
    |Key2(MaxKeySize)|StartPosition(64)|
    |Key3(MaxKeySize)|StartPosition(64)|
    ...
   //Key1's values
    |ValueSize(32)|Value(ValueSize)|
    ...
    |-1| // -1 as a end flag.
   //Key2's values
   ...





*/
class MapperResultReader : public ReduceInput {
  public:
    MapperResultReader(std::unique_ptr<AFileReader> reader,
                       const std::string           &key);
    ~MapperResultReader() {}

  public:
    const std::string key();
    bool              done();
    const std::string value();
    void              NextValue();

  public:
    void resetKey(const std::string key);

  private:
    void readHeader();

  private:
    std::unique_ptr<AFileReader>  _reader;
    std::string                   _key;
    std::map<std::string, size_t> _pos_map;
    bool                          _has_next;
};
class MapperResultWriter {};

} // namespace mapreduce
} // namespace pglang
