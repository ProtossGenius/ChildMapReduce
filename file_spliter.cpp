#include "file_spliter.h"
#include <fstream>
#include <iostream>
namespace pglang {
namespace mapreduce {

FileSpliter::FileSpliter(std::unique_ptr<AFileReader> file, size_t size)
    : _end_checker(std::make_shared<EndChecker>())
    , _size(size) {
    _file_reader = std::move(file);
}

void find_end_pos(std::string                  &buffer,
                  std::unique_ptr<AFileReader> &_file_reader,
                  std::shared_ptr<EndChecker> _end_checker, size_t &cur);
void FileSpliter::split(cb_file_split split) {
    size_t      fileSize = _file_reader->size();
    size_t      lastPos = 0, cur = 0;
    std::string buffer;
    buffer.resize(_end_checker->bufferSize());
    while (lastPos < fileSize) {
        _file_reader->seekg(std::min(lastPos + _size, fileSize));
        cur = _file_reader->tellg();
        if (cur == lastPos) break;
        find_end_pos(buffer, _file_reader, _end_checker, cur);
        split(_file_reader->uri() + "|" + std::to_string(lastPos) + "|" +
              std::to_string(cur));

        lastPos = cur + 1;
        _file_reader->seekg(lastPos);
    }
}

void find_end_pos(std::string                  &buffer,
                  std::unique_ptr<AFileReader> &_file_reader,
                  std::shared_ptr<EndChecker> _end_checker, size_t &cur) {
    _end_checker->clean();
    while (_end_checker->next()) {
        auto size = _file_reader->read(buffer);
        if (size == 0) break;
        for (int i = 0; i < size && _end_checker->next(); ++i, ++cur) {
            _end_checker->read(buffer[ i ]);
        }
    }
}
} // namespace mapreduce
} // namespace pglang
