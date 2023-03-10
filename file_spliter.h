#pragma once
#include "file_reader.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
// @SMIST  Doc("Split big file to many slice")

namespace pglang {
namespace mapreduce {

class EndChecker {
  public:
    bool         next() const { return _next; }
    virtual void read(char c) { _next = false; }
    virtual void clean() { _next = true; }

  public:
    virtual ~EndChecker() {}

  protected:
    bool _next = true;
};

typedef std::function<void(const std::string sliceInfo)> cb_file_split;
class FileSpliter {
  public:
    FileSpliter(std::unique_ptr<AFileReader> file, size_t size);
    void setEndChecker(std::unique_ptr<EndChecker> end_checker) {
        _end_checker = std::move(end_checker);
    }
    void split(cb_file_split split);

  private:
    std::unique_ptr<AFileReader> _file_reader;
    std::unique_ptr<EndChecker>  _end_checker;
    size_t                       _size;
};

} // namespace mapreduce
} // namespace pglang
