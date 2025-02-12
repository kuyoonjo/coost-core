#pragma once

#if !defined(_WIN32) && !defined(__linux__)
#include "../hook.h"
#include "../sock_ctx.h"
#include <coost/co/co.h>
#include <coost/log.h>
#include <sys/event.h>
#include <time.h>

namespace coost {
namespace co {

class Kqueue {
public:
  Kqueue(int sched_id);
  ~Kqueue();

  bool add_ev_read(int fd, void *ud);
  bool add_ev_write(int fd, void *ud);
  void del_ev_read(int fd);
  void del_ev_write(int fd);

  /**
   * remove all events on the socket from kqueue
   *   - When a socket was closed, events on that socket will be removed from
   *     kqueue automatically, and we needn't call del_event() manually.
   */
  void del_event(int fd);

  int wait(int ms) {
    if (ms >= 0) {
      struct timespec ts = {ms / 1000, ms % 1000 * 1000000};
      return __sys_api(kevent)(_kq, 0, 0, _ev, 1024, &ts);
    } else {
      return __sys_api(kevent)(_kq, 0, 0, _ev, 1024, 0);
    }
  }

  void signal(char c = 'x') {
    if (atomic_bool_cas(&_signaled, 0, 1, coost::mo_acq_rel,
                        coost::mo_acquire)) {
      const int r = (int)__sys_api(write)(_pipe_fds[1], &c, 1);
      if (r != 1)
        COOST_LOG_ERROR("pipe write error..");
    }
  }

  const struct kevent &operator[](int i) const { return _ev[i]; }
  void *user_data(const struct kevent &ev) { return ev.udata; }
  bool is_ev_pipe(const struct kevent &ev) { return ev.udata == 0; }
  void handle_ev_pipe();
  void close();

private:
  int _kq;
  int _pipe_fds[2];
  int _signaled;
  struct kevent *_ev;
};

typedef struct kevent epoll_event;
typedef Kqueue Epoll;

} // namespace co
} // namespace coost
#endif
