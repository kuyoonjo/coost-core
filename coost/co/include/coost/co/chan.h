#pragma once

#include <coost/macro.h>
#include <functional>

namespace coost {
namespace co {
namespace xx {

class pipe {
public:
  typedef std::function<void(void *, void *, int)> C;
  typedef std::function<void(void *)> D;

  pipe(uint32_t buf_size, uint32_t blk_size, uint32_t ms, C &&c, D &&d);
  ~pipe();

  pipe(pipe &&p) noexcept : _p(p._p) { p._p = 0; }

  pipe(const pipe &p);

  void operator=(const pipe &) = delete;

  void read(void *p) const;
  void write(void *p, int o) const;
  void close() const;
  bool is_closed() const;
  bool done() const;

private:
  void *_p;
};

} // namespace xx

// Implement of channel in golang, it was improved a lot since v3.0.1:
//   - `T` can be non-POD types (std::string, e.g.).
//   - It can be used in coroutines and/or non-coroutines.
//   - Channel can be closed (write disabled, read ok if not empty).
template <typename T> class chan {
public:
  // @cap  max capacity of the queue, 1 by default.
  // @ms   timeout in milliseconds, -1 by default.
  explicit chan(uint32_t cap = 1, uint32_t ms = (uint32_t)-1)
      : _p(
            cap * sizeof(T), sizeof(T), ms,
            [](void *dst, void *src, int o) {
              switch (o) {
              case 0:
                new (dst) T(*static_cast<const T *>(src));
                break;
              case 1:
                new (dst) T(std::move(*static_cast<T *>(src)));
                break;
              }
            },
            [](void *p) { static_cast<T *>(p)->~T(); }) {}

  ~chan() = default;

  chan(chan &&c) : _p(std::move(c._p)) {}
  chan(const chan &c) : _p(c._p) {}
  void operator=(const chan &) = delete;

  // read an element from the channel to @x
  chan &operator>>(T &x) const {
    _p.read((void *)&x);
    return (chan &)*this;
  }

  // write an element to the channel (copy constructor will be used)
  chan &operator<<(const T &x) const {
    _p.write((void *)&x, 0);
    return (chan &)*this;
  }

  // write an element to the channel (move constructor will be used)
  chan &operator<<(T &&x) const {
    _p.write((void *)&x, 1);
    return (chan &)*this;
  }

  // return true if the read or write operation was done successfully
  bool done() const { return _p.done(); }

  // close the channel.
  // write was disabled then, but we can still read from the channel.
  void close() const { _p.close(); }

  // check if the channel was closed (false for closed)
  explicit operator bool() const { return !_p.is_closed(); }

private:
  xx::pipe _p;
};

template <typename T> using Chan = chan<T>;

} // namespace co

} // namespace coost