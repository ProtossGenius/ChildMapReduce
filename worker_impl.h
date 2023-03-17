#pragma once
#include <functional>
#include <memory>
#include <string>
namespace pglang {
namespace mapreduce {
class Mapper;
class Reducer;

class Worker {
  public:
  public:
    virtual ~Worker() {}

  public:
    void MapperWork(const std::string &workInfo);
    void ReducerWork(const std::string &workInfo);

  public:
    void setMapper(std::shared_ptr<Mapper> mapper) { this->_mapper = mapper; }
    void setReducer(std::shared_ptr<Reducer> reducer) {
        this->_reducer = reducer;
    }
    std::shared_ptr<Mapper>  getMapper() { return _mapper; }
    std::shared_ptr<Reducer> getReducer() { return _reducer; }

  public:
    // type : MAPPER OR REDUCER; path is file's path
    void Accept(const std::string &type, const std::string &path,
                const std::string &workInfo);
    void Clean();
    void ReduceDone(
        std::function<void(const std::string &, const std::string &)> onFinish);

  private:
    virtual void AcceptMapper(const std::string &path)  = 0;
    virtual void AcceptReducer(const std::string &path) = 0;

  private:
    std::shared_ptr<Mapper>  _mapper;
    std::shared_ptr<Reducer> _reducer;
};

Worker *createWorker();
} // namespace mapreduce

} // namespace pglang
