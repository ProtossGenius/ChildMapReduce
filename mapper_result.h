#pragma once
#include "file_reader.h"
#include "mapreduce.h"
#include <fstream>
#include <memory>
#include <string>
namespace pglang {
namespace mapreduce {
/* MapperResult file format
#head
    |KeyMaxLen(32)|KeyNum(32)|
#key-pos table
    |Key1(MaxKeySize)|StartPosition(64)|
    |Key2(MaxKeySize)|StartPosition(64)|
    |Key3(MaxKeySize)|StartPosition(64)|
#data
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
    ~MapperResultReader() { _reader->close(); }

  public:
    const std::string key() { return _key; }
    bool              done() { return _done; }
    std::string       value() { return _value; }
    void              NextValue();

  public:
    void resetKey(const std::string &key);

  private:
    void readHeader();

  private:
    std::unique_ptr<AFileReader>  _reader;
    std::string                   _key;
    std::map<std::string, size_t> _pos_map;
    std::string                   _value;
    bool                          _done;
};

void write_mapper_result(const std::string &file, const MapperResult &result);

} // namespace mapreduce
} // namespace pglang
  //
