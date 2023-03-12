#include "mapper_result.h"
#include "defer.h"
#include <cstring>
#include <iostream>
namespace pglang {
namespace mapreduce {
MapperResultReader::MapperResultReader(std::unique_ptr<AFileReader> reader,
                                       const std::string           &key)
    : _reader(std::move(reader))
    , _key(key)
    , _done(false)

{
    if (_reader->size() == 0)
        throw std::runtime_error(
            "MapperResultReader need file not empty, file: " + _reader->uri());
    readHeader();
    resetKey(key);
    NextValue();
}

inline int read_int(std::unique_ptr<AFileReader> &_reader) {
    ftype<int> len;
    _reader->readsome(len.arr, 4);
    return len.value;
}

inline size_t read_size_t(std::unique_ptr<AFileReader> &_reader) {
    ftype<size_t> len;
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
    if (_done) {
        throw std::runtime_error("MapperResultReader has no next value. did "
                                 "you check function done() ?");
    }
    int _next_size = read_int(_reader);
    if (_next_size == -1) {
        _done = true;
        return;
    }
    _value.resize(_next_size);
    _reader->read(_value);
}

void MapperResultReader::resetKey(const std::string &key) {
    if (_pos_map.count(key) == 0) {
        _done = true;
        return;
    }
    _reader->seekg(_pos_map[ key ]);
}

void write_info_key(std::ofstream &of, const KeysResult &result);

void write_mapper_result(const std::string &path, const MapperResult &result) {
    for (auto &[ hash, kmap ] : result) {
        std::ofstream of(path + "." + std::to_string(hash), std::ios::out);
        write_info_key(of, kmap);
    }
}

template <typename type> void write(std::ofstream &of, type t) {
    ftype<type> v;
    v.value = t;
    of.write(v.arr, sizeof(v));
}

// file format in mapper_result.h
void write_info_key(std::ofstream &of, const KeysResult &result) {
    int key_nums    = 0;
    int key_max_len = 0;
    for (const auto &[ key, _ ] : result) {
        ++key_nums;
        key_max_len = std::max(int(key.size()), key_max_len);
    }

    key_max_len += 1;
    char *buffer = new char[ key_max_len ]{};
    Defer free_buffer([ buffer ]() { delete[] buffer; });
    write(of, key_max_len);
    write(of, key_nums);
    of.flush();
    std::map<std::string, size_t> keyPosTable;
    // skip key-pos table;
    of.seekp(8 + key_nums * (key_max_len + 8));
    // write data
    for (const auto &[ key, values ] : result) {
        keyPosTable[ key ] = of.tellp();
        for (const auto &value : values) {
            write(of, int(value.size()));
            of.write(value.c_str(), value.size());
        }
        write(of, int(-1));
    }
    of.seekp(8);
    // write key-pos table;
    for (const auto &[ key, pos ] : keyPosTable) {
        strcpy(buffer, key.c_str());
        of.write(buffer, key_max_len);
        write(of, pos);
    }
    of.flush();
    of.close();
}

} // namespace mapreduce
} // namespace pglang
