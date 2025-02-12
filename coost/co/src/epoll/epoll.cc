#ifdef __linux__
#include "epoll.h"
#include "../close.h"

namespace coost {
namespace co {

Epoll::Epoll(int sched_id) : _signaled(0), _sched_id(sched_id) {
  _ep = epoll_create(1024);
  if (_ep == -1)
    COOST_LOG_FATAL("epoll create error: ", coost::strerror());
  co::set_cloexec(_ep);

  auto r = __sys_api(pipe)(_pipe_fds);
  if (r == -1)
    COOST_LOG_FATAL("create pipe error: ", coost::strerror());
  co::set_cloexec(_pipe_fds[0]);
  co::set_cloexec(_pipe_fds[1]);

  // register ev_read for _pipe_fds[0] to this epoll.
  co::set_nonblock(_pipe_fds[0]);
  this->add_ev_read(_pipe_fds[0], 0);
  // CHECK(this->add_ev_read(_pipe_fds[0], 0));

  _ev = (epoll_event *)::calloc(1024, sizeof(epoll_event));
}

Epoll::~Epoll() {
  this->close();
  if (_ev) {
    ::free(_ev);
    _ev = 0;
  }
}

bool Epoll::add_ev_read(int fd, int32_t co_id) {
  if (fd < 0)
    return false;
  auto &ctx = co::get_sock_ctx(fd);
  if (ctx.has_ev_read())
    return true; // already exists

  const bool has_ev_write = ctx.has_ev_write(_sched_id);
  epoll_event ev;
  ev.events =
      has_ev_write ? (EPOLLIN | EPOLLOUT | EPOLLET) : (EPOLLIN | EPOLLET);
  ev.data.fd = fd;

  const int r =
      epoll_ctl(_ep, has_ev_write ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, fd, &ev);
  if (r == 0) {
    ctx.add_ev_read(_sched_id, co_id);
    return true;
  } else {
    COOST_LOG_ERROR("epoll add ev read error: ", coost::strerror(),
                    ", fd: ", fd, ", co: ", co_id);
    return false;
  }
}

bool Epoll::add_ev_write(int fd, int32_t co_id) {
  if (fd < 0)
    return false;
  auto &ctx = co::get_sock_ctx(fd);
  if (ctx.has_ev_write())
    return true; // already exists

  const bool has_ev_read = ctx.has_ev_read(_sched_id);
  epoll_event ev;
  ev.events =
      has_ev_read ? (EPOLLIN | EPOLLOUT | EPOLLET) : (EPOLLOUT | EPOLLET);
  ev.data.fd = fd;

  const int r =
      epoll_ctl(_ep, has_ev_read ? EPOLL_CTL_MOD : EPOLL_CTL_ADD, fd, &ev);
  if (r == 0) {
    ctx.add_ev_write(_sched_id, co_id);
    return true;
  } else {
    COOST_LOG_ERROR("epoll add ev write error: ", coost::strerror(),
                    ", fd: ", fd, ", co: ", co_id);
    return false;
  }
}

void Epoll::del_ev_read(int fd) {
  if (fd < 0)
    return;
  auto &ctx = co::get_sock_ctx(fd);
  if (!ctx.has_ev_read())
    return; // not exists

  int r;
  ctx.del_ev_read();
  if (!ctx.has_ev_write(_sched_id)) {
    r = epoll_ctl(_ep, EPOLL_CTL_DEL, fd, (epoll_event *)8);
  } else {
    epoll_event ev;
    ev.events = EPOLLOUT | EPOLLET;
    ev.data.fd = fd;
    r = epoll_ctl(_ep, EPOLL_CTL_MOD, fd, &ev);
  }

  if (r != 0 && errno != ENOENT) {
    COOST_LOG_ERROR("epoll del ev_read error: ", coost::strerror(),
                    ", fd: ", fd);
  }
}

void Epoll::del_ev_write(int fd) {
  if (fd < 0)
    return;
  auto &ctx = co::get_sock_ctx(fd);
  if (!ctx.has_ev_write())
    return; // not exists

  int r;
  ctx.del_ev_write();
  if (!ctx.has_ev_read(_sched_id)) {
    r = epoll_ctl(_ep, EPOLL_CTL_DEL, fd, (epoll_event *)8);
  } else {
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    r = epoll_ctl(_ep, EPOLL_CTL_MOD, fd, &ev);
  }

  if (r != 0 && errno != ENOENT) {
    COOST_LOG_ERROR("epoll del ev_write error: ", coost::strerror(),
                    ", fd: ", fd);
  }
}

void Epoll::del_event(int fd) {
  if (fd < 0)
    return;
  auto &ctx = co::get_sock_ctx(fd);
  if (ctx.has_event()) {
    ctx.del_event();
    const int r = epoll_ctl(_ep, EPOLL_CTL_DEL, fd, (epoll_event *)8);
    if (r != 0)
      COOST_LOG_ERROR("epoll del event error: ", coost::strerror(),
                      ", fd: ", fd);
  }
}

inline void closesocket(int &fd) {
  if (fd >= 0) {
    _close_nocancel(fd);
    fd = -1;
  }
}

void Epoll::close() {
  co::closesocket(_ep);
  co::closesocket(_pipe_fds[0]);
  co::closesocket(_pipe_fds[1]);
}

void Epoll::handle_ev_pipe() {
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
