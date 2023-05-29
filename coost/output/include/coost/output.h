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

// print to fp (thread-safe)
//   - coost::output(fp, "hello");
template <typename... X> inline void output(FILE *fp, X &&...x) {
  static auto m = mem::_make_rootic<std::mutex>();
  static __thread fastream *s = 0;
  if (!s)
    s = mem::_make_rootic<fastream>(256);
  auto n = s->size();
  (((*s) << std::forward<X>(x)), ...);

  std::lock_guard<std::mutex> lg(*m);
  ::fwrite(s->data() + n, 1, s->size() - n, fp);
  s->resize(n);
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
