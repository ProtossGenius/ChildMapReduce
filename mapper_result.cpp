#include "mapper_result.h"
#include "defer.h"
namespace pglang {
namespace mapreduce {
MapperResultReader::MapperResultReader(std::unique_ptr<AFileReader> reader,
                                       const std::string           &key)
    : _reader(std::move(reader))
    , _key(key)

{
    readHeader();
    resetKey(key);
}

inline int read_int(std::unique_ptr<AFileReader> &_reader) {
    f_int32 len;
    _reader->readsome(len.arr, 4);
    return len.value;
}

inline int read_size_t(std::unique_ptr<AFileReader> &_reader) {
    f_size_t len;
    _reader->readsome(len.arr, 8);
    return len.value;
}
void MapperResultReader::readHeader() {
    int keyMaxLen = read_int(_reader);

    int   keyNums = read_int(_reader);
    char *buffer  = new char[ keyMaxLen ];
    Defer del_buffer([ = ]() { delete[] buffer; });
    for (int i = 0; i < keyNums; ++i) {
        _reader->readsome(buffer, keyMaxLen);
        size_t position    = read_size_t(_reader);
        _pos_map[ buffer ] = position;
    }
}

void MapperResultReader::NextValue() {
    if (_next_size < 0) {
        throw std::runtime_error("MapperResultReader has no next value. did "
                                 "you check function done() ?");
    }

    _value.resize(_next_size);
    _reader->read(_value);
    _next_size = read_int(_reader);
}

void MapperResultReader::resetKey(const std::string &key) {
    if (_pos_map.count(key) == 0) {
        _next_size = -1;
        return;
    }
    _begin     = _pos_map[ key ];
    _next_size = read_int(_reader);
}

} // namespace mapreduce
} // namespace pglang
