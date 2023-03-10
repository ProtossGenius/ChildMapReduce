#pragma once
#include <functional>
namespace pglang {
namespace mapreduce {
class Defer {
  public:
    Defer(std::function<void()> defer)
        : _defer(defer) {}
    ~Defer() { _defer(); }

  private:
    std::function<void()> _defer;
};

} // namespace mapreduce
} // namespace pglang
