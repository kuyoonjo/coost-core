#pragma once

#include <coost/fastream.h>
#include <iostream>
#include <mutex>

namespace coost {

#ifdef _WIN32
constexpr auto endl = "\r\n";
#else
constexpr auto endl = "\n";
#endif

namespace xx {
static inline std::mutex output_mutex;
static inline thread_local fastream output_fastream(256);
} // namespace xx

// print to fp (thread-safe)
//   - coost::output(fp, "hello");
template <typename... X> inline void output(FILE *fp, X &&...x) {
  auto n = xx::output_fastream.size();
  ((xx::output_fastream << std::forward<X>(x)), ...);
  {
    std::lock_guard<std::mutex> lg(xx::output_mutex);
    ::fwrite(xx::output_fastream.data() + n, 1, xx::output_fastream.size() - n, fp);
    xx::output_fastream.resize(n);
  }
}

// print to stdout (thread-safe)
//   - coost::print("hello");
template <typename... X> inline void print(X &&...x) { output(stdout, x...); }
// print to stdout with newline (thread-safe)
//   - coost::println("hello");
template <typename... X> inline void println(X &&...x) {
  output(stdout, x..., endl);
}

// print to stderr (thread-safe)
//   - coost::eprint("hello");
template <typename... X> inline void eprint(X &&...x) { output(stderr, x...); }
// print to stderr with newline (thread-safe)
//   - coost::eprintln("hello");
template <typename... X> inline void eprintln(X &&...x) {
  output(stderr, x..., endl);
}

} // namespace coost
