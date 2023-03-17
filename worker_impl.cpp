#include "worker_impl.h"
#include "mapreduce.h"
namespace pglang {
namespace mapreduce {
void Worker::Accept(const std::string &type, const std::string &path,
                    const std::string &workInfo) {
    if (type == "MAPPER") {
        AcceptMapper(path);
        MapperWork(workInfo);
    } else {
        AcceptReducer(path);
        ReducerWork(workInfo);
    }
}
void Worker::MapperWork(const std::string &workInfo) {
    _mapper->MapperWork(workInfo);
}

void Worker::ReducerWork(const std::string &workInfo) {
    _reducer->ReducerWork(workInfo);
}
void Worker::ReduceDone(CBOnReduceFinish onFinish) { _reducer->Done(onFinish); }
class LocalWorker : public Worker {
  public:
    void AcceptReducer(const std::string &path) override {}
    void AcceptMapper(const std::string &path) override {}
};

Worker *createWorker() { return new LocalWorker(); }
} // namespace mapreduce
} // namespace pglang
