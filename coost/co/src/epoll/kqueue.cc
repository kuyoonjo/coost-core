#if !defined(_WIN32) && !defined(__linux__)
#include "kqueue.h"
#include "../close.h"

namespace coost {
namespace co {

Kqueue::Kqueue(int sched_id) : _signaled(0) {
  _kq = kqueue();
  if (_kq == -1)
    COOST_LOG_FATAL("kqueue create error: ", coost::strerror());
  auto ret = __sys_api(pipe)(_pipe_fds);
  if (ret == -1)
    COOST_LOG_FATAL("create pipe error: ", coost::strerror());
  co::set_cloexec(_pipe_fds[0]);
  co::set_cloexec(_pipe_fds[1]);
  co::set_nonblock(_pipe_fds[0]);
  this->add_ev_read(_pipe_fds[0], (void *)0);
  // CHECK(this->add_ev_read(_pipe_fds[0], (void*)0));
  _ev = (struct kevent *)::calloc(1024, sizeof(struct kevent));
  (void)sched_id;
}

Kqueue::~Kqueue() {
  this->close();
  if (_ev) {
    ::free(_ev);
    _ev = 0;
  }
}

bool Kqueue::add_ev_read(int fd, void *p) {
  if (fd < 0)
    return false;
  auto &ctx = co::get_sock_ctx(fd);
  if (ctx.has_ev_read())
    return true; // already exists

  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, p);

  if (__sys_api(kevent)(_kq, &event, 1, 0, 0, 0) == 0) {
    ctx.add_ev_read();
    return true;
  } else {
    COOST_LOG_ERROR("kqueue add ev_read error: ", coost::strerror(),
                    ", fd: ", fd);
    return false;
  }
}

bool Kqueue::add_ev_write(int fd, void *p) {
  if (fd < 0)
    return false;
  auto &ctx = co::get_sock_ctx(fd);
  if (ctx.has_ev_write())
    return true; // already exists

  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_ADD, 0, 0, p);

  if (__sys_api(kevent)(_kq, &event, 1, 0, 0, 0) == 0) {
    ctx.add_ev_write();
    return true;
  } else {
    COOST_LOG_ERROR("kqueue add ev_write error: ", coost::strerror(),
                    ", fd: ", fd);
    return false;
  }
}

void Kqueue::del_ev_read(int fd) {
  if (fd < 0)
    return;
  auto &ctx = co::get_sock_ctx(fd);
  if (!ctx.has_ev_read())
    return;

  ctx.del_ev_read();
  struct kevent event;
  EV_SET(&event, fd, EVFILT_READ, EV_DELETE, 0, 0, 0);

  if (__sys_api(kevent)(_kq, &event, 1, 0, 0, 0) != 0) {
    COOST_LOG_ERROR("kqueue del ev_read error: ", coost::strerror(),
                    ", fd: ", fd);
  }
}

void Kqueue::del_ev_write(int fd) {
  if (fd < 0)
    return;
  auto &ctx = co::get_sock_ctx(fd);
  if (!ctx.has_ev_write())
    return;

  ctx.del_ev_write();
  struct kevent event;
  EV_SET(&event, fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);

  if (__sys_api(kevent)(_kq, &event, 1, 0, 0, 0) != 0) {
    COOST_LOG_ERROR("kqueue del ev_write error: ", coost::strerror(),
                    ", fd: ", fd);
  }
}

void Kqueue::del_event(int fd) {
  if (fd < 0)
    return;
  auto &ctx = co::get_sock_ctx(fd);
  if (!ctx.has_event())
    return;

  int i = 0;
  struct kevent event[2];
  if (ctx.has_ev_read())
    EV_SET(&event[i++], fd, EVFILT_READ, EV_DELETE, 0, 0, 0);
  if (ctx.has_ev_write())
    EV_SET(&event[i++], fd, EVFILT_WRITE, EV_DELETE, 0, 0, 0);

  ctx.del_event();
  if (__sys_api(kevent)(_kq, event, i, 0, 0, 0) != 0) {
    COOST_LOG_ERROR("kqueue del event error: ", coost::strerror(),
                    ", fd: ", fd);
  }
}

inline void closesocket(int &fd) {
  if (fd >= 0) {
    _close_nocancel(fd);
    fd = -1;
  }
}

void Kqueue::close() {
  co::closesocket(_kq);
  co::closesocket(_pipe_fds[0]);
  co::closesocket(_pipe_fds[1]);
}

void Kqueue::handle_ev_pipe() {
  int32_t dummy;
  while (true) {
    int r = __sys_api(read)(_pipe_fds[0], &dummy, 4);
    if (r != -1) {
      if (r < 4)
        break;
      continue;
    } else {
      if (errno == EWOULDBLOCK || errno == EAGAIN)
        break;
      if (errno == EINTR)
        continue;
      COOST_LOG_ERROR("pipe read error: ", coost::strerror(),
                      ", fd: ", _pipe_fds[0]);
      break;
    }
  }
  atomic_store(&_signaled, 0, mo_release);
}

} // namespace co
} // namespace coost
#endif
