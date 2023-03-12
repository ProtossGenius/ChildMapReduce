#include "file_reader.h"
#include <iostream>
namespace pglang {
namespace mapreduce {
LocalFileReader::LocalFileReader(const std::string &path, size_t size)
    : _uri(path)
    , _reader(path, std::ios::in)
    , _buffer(nullptr) {
    setBufferSize(size);
}

size_t LocalFileReader::size() {
    size_t current = _reader.tellg();
    _reader.seekg(0, _reader.end);
    int size = _reader.tellg();
    _reader.seekg(current);
    return size < 0 ? 0 : size;
}
size_t LocalFileReader::read(std::string &inp) {
    size_t left_size = inp.size(), ptr = 0;
    while (left_size > 0) {
        auto read_size =
            _reader.readsome(_buffer, std::min(_buffer_size, left_size));
        if (read_size == 0) {
            break;
        }
        for (int i = 0; i < read_size; ++i) {
            inp[ ptr++ ] = _buffer[ i ];
        }
        left_size -= read_size;
    }

    return inp.size() - left_size;
}
void LocalFileReader::setBufferSize(size_t size) {
    _buffer_size = size;
    if (_buffer != nullptr) delete[] _buffer;
    _buffer = new char[ size ];
}

} // namespace mapreduce
} // namespace pglang
