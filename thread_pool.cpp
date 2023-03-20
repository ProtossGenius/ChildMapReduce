#include "thread_pool.h"

namespace pglang {
namespace mapreduce {
ThreadPool::ThreadPool(size_t threadNum)
    : _iosv()
    , _work(std::make_shared<io_service::work>(_iosv)) {
    for (int i = 0; i < threadNum; ++i) {
        _work_group.create_thread([ this ]() { this->run(); });
    }
}

void ThreadPool::run() { _iosv.run(); }

} // namespace mapreduce
} // namespace pglang
