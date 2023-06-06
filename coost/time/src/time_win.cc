#ifdef _WIN32

#include <coost/time.h>
#include <time.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace coost {
namespace time {
namespace now {
namespace xx {

inline int64_t _query_counts() {
  LARGE_INTEGER x;
  QueryPerformanceCounter(&x);
  return x.QuadPart;
}

inline const int64_t &_counts_per_sec() {
  static const int64_t freq = []() {
    LARGE_INTEGER x;
    QueryPerformanceFrequency(&x);
    return x.QuadPart;
  }();
  return freq;
}

inline int64_t ns() {
  const int64_t count = _query_counts();
  const int64_t &freq = _counts_per_sec();
  return (int64_t)(static_cast<double>(count) * 1000000000 / freq);
}

inline int64_t us() {
  const int64_t count = _query_counts();
  const int64_t &freq = _counts_per_sec();
  return (int64_t)(static_cast<double>(count) * 1000000 / freq);
}

inline int64_t ms() {
  const int64_t count = _query_counts();
  const int64_t &freq = _counts_per_sec();
  return (int64_t)(static_cast<double>(count) * 1000 / freq);
}

} // namespace xx

int64_t ns() { return xx::ns(); }

int64_t us() { return xx::us(); }

int64_t ms() { return xx::ms(); }

std::string str(const char *fm) {
  int64_t x = ::time(0);
  struct tm t;
  _localtime64_s(&t, &x);

  char buf[256];
  const size_t r = strftime(buf, sizeof(buf), fm, &t);
  return std::string(buf, buf + r);
}

} // namespace now

namespace epoch {

inline int64_t filetime() {
  FILETIME ft;
  LARGE_INTEGER x;
  GetSystemTimeAsFileTime(&ft);
  x.LowPart = ft.dwLowDateTime;
  x.HighPart = ft.dwHighDateTime;
  return x.QuadPart - 116444736000000000ULL;
}

int64_t ms() { return filetime() / 10000; }

int64_t us() { return filetime() / 10; }

} // namespace epoch

namespace sleep {

void ms(uint32_t n) { ::Sleep(n); }

void sec(uint32_t n) { ::Sleep(n * 1000); }

} // namespace sleep
} // namespace time

} // namespace coost
#endif
