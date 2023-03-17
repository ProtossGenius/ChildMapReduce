#pragma once
#include "leveldb_tools.h"
#include <functional>
#include <map>
#include <memory>
#include <queue>
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
class Worker;
typedef std::unordered_map<size_t, KeysResult> MapperResult;
typedef std::function<void(const std::string &filePath, const MapperResult &)>
    CBOnMapperFinish;
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

    void setOnFinish(CBOnMapperFinish onFinish) {
        if (onFinish != nullptr) _on_finish = onFinish;
    }

    void reset(const std::string &name, int partSize,
               const std::string &pathPre) {
        _part_size = partSize, _path_pre = pathPre, _name = name;
        _result.clear();
    }
    void reset(const std::string &name) { reset(name, _part_size, _path_pre); }

  private:
    // only use in worker.
    void MapperWork(const std::string &workInfo);
    friend class Worker;

  private:
    MapperResult     _result;
    size_t           _part_size; // Hash's num
    std::string      _path_pre;  // out file's path
    std::string      _name;      // mapper's name, for log
    CBOnMapperFinish _on_finish;
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

typedef std::function<void(const std::string &key, const std::string &value)>
    CBOnReduceFinish;

class Reducer {
  public:
    Reducer(const std::string &cachePath);

  public:
    void               Emit(const std::string &value);
    const std::string &GetValue() { return _value; }
    virtual ~Reducer() {}

  protected:
    virtual void Reduce(ReduceInput *input) = 0;

  private:
    void ReducerWork(const std::string &workInfo);
    void Done(CBOnReduceFinish); // Reducer read finish;
    friend class Worker;

  private:
    std::string                  _path;
    std::string                  _key;
    std::string                  _value;
    std::unique_ptr<leveldb::DB> _cache;
};

} // namespace mapreduce
} // namespace pglang
