#pragma once
#include <functional>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
namespace pglang {
namespace mapreduce {

// User interface, some interface user needs known.

class MapInput {
  public:
    MapInput(const std::string &value)
        : _value(value) {}
    MapInput(std::string &&value)
        : _value(std::move(value)) {}

  public:
    const std::string value() const { return _value; }

  private:
    std::string _value;
};

typedef std::map<std::string, std::vector<std::string>> KeysResult;

typedef std::unordered_map<size_t, KeysResult> MapperResult;
class Mapper {
  public:
    Mapper(int part_size, const std::string &path_pre, const std::string &name);

  public:
    // Map deal input, to K-V pairs;
    virtual void Map(const MapInput &input) = 0;
    virtual ~Mapper() {}
    virtual size_t hash(const std::string &inp) const {
        return std::hash<std::string>{}(inp);
    }

  public:
    // Emit the K-V pairs;
    void Emit(const std::string &key, const std::string &value);

  public: // only use in worker.
    void MapperWork(const std::string &workInfo);

  private:
    MapperResult _result;
    size_t       _part_size;
    std::string  _path_pre;
    std::string  _name;
};

class ReduceInput {
  public:
    virtual ~ReduceInput() {}

  public:
    virtual const std::string key()       = 0;
    virtual bool              done()      = 0;
    virtual std::string       value()     = 0;
    virtual void              NextValue() = 0;
};

class Reducer {
  public:
    virtual void Reduce(ReduceInput *input) = 0;
    void         Emit(const std::string &value);
    virtual ~Reducer();
};

} // namespace mapreduce
} // namespace pglang
