#pragma once
#include "file.h"
#include <functional>
#include <string>
namespace pglang {
namespace mapreduce {

// a abstract file, may use net-file in future.
class AFileReader {
  public:
    virtual ~AFileReader() {}
    AFileReader() {}
    AFileReader(const AFileReader &) = delete;

  public:
    // a string to access file.
    virtual std::string uri() const = 0;
    // go to file's random position.
    virtual void seekg(size_t pos) = 0;
    // pointer's position.
    virtual size_t tellg() = 0;
    // file's size.
    virtual size_t size()                              = 0;
    virtual size_t read(std::string &inp)              = 0;
    virtual size_t readsome(char *buffer, size_t size) = 0;
    virtual void   close()                             = 0;
};

// not thread safe in multi-thread env.
class LocalFileReader : public AFileReader {
  public:
    LocalFileReader(const std::string &path, size_t buffer_size = 16 * FSIZE_K);

  public:
    std::string uri() const { return _uri; }
    void        seekg(size_t pos) { _reader.seekg(pos); }
    size_t      tellg() { return _reader.tellg(); }
    size_t      size();
    size_t      read(std::string &inp);
    void        close() { _reader.close(); }
    size_t      readsome(char *buffer, size_t size) {
             return _reader.readsome(buffer, size);
    }

  public:
    // should not change morethan one times.
    void setBufferSize(size_t size);

  public:
    ~LocalFileReader() {
        if (_buffer != nullptr) delete[] _buffer;
        _reader.close();
    }

  private:
    std::string   _uri;
    std::ifstream _reader;
    size_t        _buffer_size;
    char         *_buffer;
};
} // namespace mapreduce
} // namespace pglang
