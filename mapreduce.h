#pragma once
#include <functional>
#include <map>
#include <string>
namespace pglang {
namespace mapreduce {

// User interface, some interface user needs known.

class MapInput {
  public:
    const std::string value() const;
};
class Mapper {
  public:
    // Map deal input, to K-V pairs;
    virtual void Map(const MapInput &input) = 0;
    // Emit the K-V pairs;
    void Emit(const std::string &key, const std::string &value);
    virtual ~Mapper();
};

class ReduceInput {
  public:
    const std::string key();
    bool              done();
    const std::string value();
    void              NextValue();
};

class Reducer {
  public:
    virtual void Reduce(ReduceInput *input) = 0;
    void         Emit(const std::string &value);
    virtual ~Reducer();
};

} // namespace mapreduce
} // namespace pglang
