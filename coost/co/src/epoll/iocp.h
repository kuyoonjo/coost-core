#ifdef _WIN32
#pragma once

#include "../hook.h"
#include "../sock_ctx.h"
#include <coost/co/co.h>
#include <coost/error.h>
#include <coost/log.h>


namespace coost {
namespace co {

class Iocp {
public:
  Iocp(int sched_id);
  ~Iocp();

  bool add_event(sock_t fd) {
    if (fd == (sock_t)-1)
      return false;
    auto &ctx = coost::co::get_sock_ctx(fd);
    if (ctx.has_event())
      return true; // already exists

    if (CreateIoCompletionPort((HANDLE)fd, _iocp, fd, 0) != 0) {
      ctx.add_event();
      return true;
    } else {
      COOST_LOG_ERROR( "iocp add socket " , fd , " error: " , coost::strerror());
      // always return true here.
      return true;
    }
  }

  // for close
  void del_event(sock_t fd) {
    if (fd != (sock_t)-1)
      coost::co::get_sock_ctx(fd).del_event();
  }

  // for half-shutdown read
  void del_ev_read(sock_t fd) {
    if (fd != (sock_t)-1)
      coost::co::get_sock_ctx(fd).del_ev_read();
  }

  // for half-shutdown write
  void del_ev_write(sock_t fd) {
    if (fd != (sock_t)-1)
      coost::co::get_sock_ctx(fd).del_ev_write();
  }

  int wait(int ms) {
    ULONG n = 0;
    const BOOL r =
        __sys_api(GetQueuedCompletionStatusEx)(_iocp, _ev, 1024, &n, ms, false);
    if (r == TRUE)
      return (int)n;
    const uint32_t e = ::GetLastError();
    if (e == WAIT_TIMEOUT)
      return 0;
    coost::error() = e;
    return -1;
  }

  void signal() {
    if (atomic_bool_cas(&_signaled, 0, 1, mo_acq_rel, mo_acquire)) {
      const BOOL r = PostQueuedCompletionStatus(_iocp, 0, 0, 0);
      if (!r) {
        const uint32_t e = ::GetLastError();
        COOST_LOG_ERROR( "PostQueuedCompletionStatus error: " , coost::strerror(e));
      }
    }
  }

  const OVERLAPPED_ENTRY &operator[](int i) const { return _ev[i]; }
  void *user_data(const OVERLAPPED_ENTRY &ev) { return ev.lpOverlapped; }
  bool is_ev_pipe(const OVERLAPPED_ENTRY &ev) { return ev.lpOverlapped == 0; }
  void handle_ev_pipe() { atomic_store(&_signaled, 0, mo_release); }

private:
  HANDLE _iocp;
  OVERLAPPED_ENTRY *_ev;
  int _signaled;
  int _sched_id;
};

typedef OVERLAPPED_ENTRY epoll_event;
typedef Iocp Epoll;

} // namespace co
} // namespace coost
#endif
