#pragma once

#include <string>

namespace coost {
namespace time {
namespace now {

// monotonic timestamp in nanoseconds
int64_t ns();

// monotonic timestamp in microseconds
int64_t us();

// monotonic timestamp in milliseconds
int64_t ms();

// "%Y-%m-%d %H:%M:%S" ==> 2023-01-07 18:01:23
std::string str(const char *fm = "%Y-%m-%d %H:%M:%S");

} // namespace now

namespace epoch {

// microseconds since epoch
int64_t us();

// milliseconds since epoch
int64_t ms();

} // namespace epoch

class Timer {
public:
  Timer() { _start = now::ns(); }

  void restart() { _start = now::ns(); }

  int64_t ns() const { return now::ns() - _start; }

  int64_t us() const { return this->ns() / 1000; }

  int64_t ms() const { return this->ns() / 1000000; }

private:
  int64_t _start;
};

namespace sleep {

void ms(uint32_t n);

void sec(uint32_t n);

} // namespace sleep
} // namespace time
} // namespace coost
