#include "file_reader.h"

namespace pglang {
namespace mapreduce {
LocalFileReader::LocalFileReader(const std::string &path, size_t size)
    : _uri(path)
    , _reader(path, std::ios::in) {
    setBufferSize(size);
}

size_t LocalFileReader::size() {
    size_t current = _reader.tellg();
    _reader.seekg(0, _reader.end);
    int size = _reader.tellg();
    _reader.seekg(current);
    return size;
}

void LocalFileReader::setBufferSize(size_t size) {
    _buffer_size = size;
    if (_buffer != nullptr) delete[] _buffer;
    _buffer = new char[ size ];
}

char *LocalFileReader::read(size_t &size) {
    size = _reader.readsome(_buffer, _buffer_size);
    return _buffer;
}

} // namespace mapreduce
} // namespace pglang
