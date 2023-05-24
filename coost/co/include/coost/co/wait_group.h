#pragma once

#include <coost/macro.h>
namespace coost {
namespace co {

class wait_group {
public:
  // initialize the counter as @n
  explicit wait_group(uint32_t n);

  // the counter is 0 by default
  wait_group() : wait_group(0) {}

  ~wait_group();

  wait_group(wait_group &&wg) noexcept : _p(wg._p) { wg._p = 0; }

  // copy constructor, just increment the reference count
  wait_group(const wait_group &wg);

  void operator=(const wait_group &) = delete;

  // increase the counter by n (1 by default)
  void add(uint32_t n = 1) const;

  // decrease the counter by 1
  void done() const;

  // blocks until the counter becomes 0
  void wait() const;

private:
  void *_p;
};

typedef wait_group WaitGroup;

} // namespace co
} // namespace coost