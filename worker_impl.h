#pragma once
#include <string>
namespace pglang {
namespace mapreduce {
class Worker {
  public:
  public:
    virtual ~Worker() {}

  public:
    void Accept(const std::string &type, const std::string &path);

  private:
    virtual void AcceptMapper(const std::string &path)  = 0;
    virtual void AcceptReducer(const std::string &path) = 0;
};

} // namespace mapreduce

} // namespace pglang
