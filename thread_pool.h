#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <functional>
namespace pglang {
namespace mapreduce {
typedef boost::asio::io_service io_service;
class ThreadPool {
  public:
    ThreadPool(size_t threadNum = 1);

  private:
    void run();

  public:
    void post(std::function<void()> func) { _iosv.post(func); }
    void JoinAll() {
        _work.reset();
        _work_group.join_all();
    }

  private:
    io_service                        _iosv;
    std::shared_ptr<io_service::work> _work;
    boost::thread_group               _work_group;
};

template <typename Type> class AsyncQueue {
  public:
    AsyncQueue(std::function<void(Type)> consumer, size_t consumerSize = 0)
        : _consumer(consumer)
        , _pool(consumerSize) {}

  public:
    void push(Type item) { _pool.post(std::bind(_consumer, item)); }

  private:
    std::function<void(Type)> _consumer;
    ThreadPool                _pool;
};

} // namespace mapreduce
} // namespace pglang
