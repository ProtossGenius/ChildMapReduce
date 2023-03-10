#include "file_spliter.h"
#include <fstream>
#include <iostream>
namespace pglang {
namespace mapreduce {

FileSpliter::FileSpliter(std::unique_ptr<AFileReader> file, size_t size)
    : _end_checker(std::make_unique<EndChecker>())
    , _size(size) {
    _file_reader = std::move(file);
}

void FileSpliter::split(cb_file_split split) {
    size_t fileSize = _file_reader->size();
    size_t lastPos = 0, cur = 0;
    char  *buffer;
    while (lastPos < fileSize) {
        _file_reader->seekg(std::min(lastPos + _size, fileSize));
        cur = _file_reader->tellg();
        if (cur == lastPos) break;
        size_t size;
        _end_checker->clean();
        while (_end_checker->next()) {
            buffer = _file_reader->read(size);
            if (size == 0) break;
            for (int i = 0; i < size && _end_checker->next(); ++i, ++cur) {
                _end_checker->read(buffer[ i ]);
            }
        }
        if (lastPos == cur) break;
        split(_file_reader->uri() + "|" + std::to_string(lastPos) + "|" +
              std::to_string(cur));

        lastPos = cur + 1;
        _file_reader->seekg(lastPos);
    }
}

} // namespace mapreduce
} // namespace pglang
