#include <coost/error.h>
#include <coost/containers/unordered_map.h>
#include <coost/fastream.h>


#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace coost {
namespace xx {

struct Error {
  Error() : e(0), s(4096) {}
  int e;
  fastream s;
  coost::unordered_map<int, uint32_t> pos;
};

inline Error &error() {
  static __thread Error *e = 0;
  return e ? *e : *(e = mem::_make_static<Error>());
}
} // namespace xx

int &error() { return xx::error().e; }

const char *strerror(int e) {
  if (e == ETIMEDOUT || e == WSAETIMEDOUT)
    return "Timed out.";

  auto &err = xx::error();
  auto it = err.pos.find(e);
  if (it != err.pos.end())
    return err.s.data() + it->second;

  uint32_t pos = (uint32_t)err.s.size();
  char *s = 0;
  FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                     FORMAT_MESSAGE_IGNORE_INSERTS,
                 0, e,
                 MAKELANGID(LANG_ENGLISH /*LANG_NEUTRAL*/, SUBLANG_DEFAULT),
                 (LPSTR)&s, 0, 0);

  if (s) {
    err.s << s << '\0';
    LocalFree(s);
    char *p = (char *)strchr(err.s.data() + pos, '\r');
    if (p)
      *p = '\0';
  } else {
    err.s << "Unknown error " << e << '\0';
  }

  err.pos[e] = pos;
  return err.s.data() + pos;
}
} // namespace coost
#else
#include <errno.h>
#include <mutex>
namespace coost {
namespace xx {

struct Error {
  Error() : s(4096) {}
  coost::fastream s;
  coost::unordered_map<int, uint32_t> pos;
};

inline Error &error() {
  static __thread Error *e = 0;
  return e ? *e : *(e = coost::mem::_make_static<Error>());
}

} // namespace xx

const char *strerror(int e) {
  if (e == ETIMEDOUT)
    return "Timed out";

  auto &err = xx::error();
  auto it = err.pos.find(e);
  if (it != err.pos.end())
    return err.s.data() + it->second;

  uint32_t pos = (uint32_t)err.s.size();
  static auto m = coost::mem::_make_static<std::mutex>();
  {
    std::lock_guard<std::mutex> g(*m);
    err.s << ::strerror(e) << '\0';
  }

  err.pos[e] = pos;
  return err.s.data() + pos;
}
} // namespace coost
#endif
