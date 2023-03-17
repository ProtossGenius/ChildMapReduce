#include "file_spliter.h"
#include "nlohmann/json.hpp"
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
void FileSpliter::split(cb_file_split on_split) {
    using json           = nlohmann::json;
    size_t      fileSize = _file_reader->size();
    size_t      begin = 0, end = 0;
    std::string buffer;
    buffer.resize(_end_checker->bufferSize());
    while (begin < fileSize) {
        _file_reader->seekg(std::min(begin + _size, fileSize));
        end = _file_reader->tellg();
        if (begin == end) break;
        find_end_pos(buffer, _file_reader, _end_checker, end);
        json info = {
            {"uri", _file_reader->uri()}, {"begin", begin}, {"end", end}};
        on_split(info.dump());
        begin = end;
        _file_reader->seekg(begin);
    }
}

void find_end_pos(std::string                  &buffer,
                  std::unique_ptr<AFileReader> &_file_reader,
                  std::shared_ptr<EndChecker> _end_checker, size_t &end) {
    _end_checker->clean();
    while (_end_checker->next()) {
        auto size = _file_reader->read(buffer);
        if (size == 0) break;
        for (int i = 0; i < size && _end_checker->next(); ++i, ++end) {
            _end_checker->read(buffer[ i ]);
        }
    }
}
} // namespace mapreduce
} // namespace pglang
