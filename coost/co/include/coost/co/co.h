#pragma once

#include "./chan.h"
#include "./event.h"
#include "./io_event.h"
#include "./mutex.h"
#include "./pool.h"
#include "./sock.h"
#include "./thread.h"
#include "./wait_group.h"
#include "closure.h"
#include <coost/containers/vector.h>

namespace coost {
namespace co {

/**
 * add a task, which will run as a coroutine
 *   - It is thread-safe and can be called from anywhere.
 *   - Closure created by new_closure() will delete itself after Closure::run()
 *     is done. Users MUST NOT delete it manually.
 *   - Closure is an abstract base class, users are free to implement their own
 *     subtype of Closure. This may be useful if users do not want a Closure to
 *     delete itself. See details in co/closure.h.
 *
 * @param cb  a pointer to a Closure created by new_closure(), or an
 * user-defined Closure.
 */
void go(Closure *cb);

/**
 * add a task, which will run as a coroutine
 *   - eg.
 *     go(f);               // void f();
 *     go([]() { ... });    // lambda
 *     go(std::bind(...));  // std::bind
 *
 *     std::function<void()> x(std::bind(...));
 *     go(x);               // std::function<void()>
 *     go(&x);              // std::function<void()>*
 *
 *   - If f is a pointer to std::function<void()>, users MUST ensure that the
 *     object f points to is valid when Closure::run() is running.
 *
 * @param f  any runnable object, as long as we can call f() or (*f)().
 */
template <typename F> inline void go(F &&f) {
  go(new_closure(std::forward<F>(f)));
}

/**
 * add a task, which will run as a coroutine
 *   - eg.
 *     go(f, 8);   // void f(int);
 *     go(f, p);   // void f(void*);   void* p;
 *     go(f, o);   // void (T::*f)();  T* o;
 *
 *     std::function<void(P)> x(std::bind(...));
 *     go(x, p);   // P p;
 *     go(&x, p);  // P p;
 *
 *   - If f is a pointer to std::function<void(P)>, users MUST ensure that the
 *     object f points to is valid when Closure::run() is running.
 *
 * @param f  any runnable object, as long as we can call f(p), (*f)(p) or
 * (p->*f)().
 * @param p  parameter of f, or a pointer to an object of class P if f is a
 * method.
 */
template <typename F, typename P> inline void go(F &&f, P &&p) {
  go(new_closure(std::forward<F>(f), std::forward<P>(p)));
}

/**
 * add a task, which will run as a coroutine
 *   - eg.
 *     go(f, o, p);   // void (T::*f)(P);  T* o;  P p;

 * @param f  a pointer to a method with a parameter in class T.
 * @param t  a pointer to an object of class T.
 * @param p  parameter of f.
 */
template <typename F, typename T, typename P>
inline void go(F &&f, T *t, P &&p) {
  go(new_closure(std::forward<F>(f), t, std::forward<P>(p)));
}

class Sched {
public:
  Sched() = delete;
  ~Sched() = delete;

  void go(Closure *cb);

  template <typename F> inline void go(F &&f) {
    this->go(new_closure(std::forward<F>(f)));
  }

  template <typename F, typename P> inline void go(F &&f, P &&p) {
    this->go(new_closure(std::forward<F>(f), std::forward<P>(p)));
  }

  template <typename F, typename T, typename P>
  inline void go(F &&f, T *t, P &&p) {
    this->go(new_closure(std::forward<F>(f), t, std::forward<P>(p)));
  }
};

class MainSched {
public:
  MainSched() = delete;
  ~MainSched() = delete;

  void loop();
};

// get all the schedulers
const coost::vector<Sched *> &scheds();

// get number of the schedulers
int sched_num();

// get the current scheduler
Sched *sched();

// get next scheduler
//   - It is useful when users want to create coroutines in the same scheduler.
//   - eg.
//     auto s = co::next_sched();
//     s->go(f);     // void f();
//     s->go(g, 7);  // void g(int);
Sched *next_sched();

// mark the main thread as a scheduler
//   - It is useful when users want to run the main thread as a scheduler.
//   - Call this function in the main function before any coroutine starts,
//     and then call MainSched::loop() with the returned result.
//   - e.g.
//     auto s = co::main_sched();
//     go(xx);    /* start coroutines here */
//     s->loop(); /* loop in main thread */
MainSched *main_sched();

// return a pointer to the current coroutine
void *coroutine();

// return id of the current scheduler, or -1 if called from non-scheduler thread
int sched_id();

// return id of the current coroutine, or -1 if called from non-coroutine
int coroutine_id();

// add a timer for the current coroutine
//   - It MUST be called in coroutine.
//   - Users MUST call yield() to suspend the coroutine after a timer was added.
//     When the timer expires, the scheduler will resume the coroutine.
void add_timer(uint32_t ms);

// add an IO event on a socket to the epoll
//   - It MUST be called in coroutine.
//   - Users MUST call yield() to suspend the coroutine after an event was
//   added.
//     When the event is present, the scheduler will resume the coroutine.
//
//   - @fd: the socket.
//   - @ev: either ev_read or ev_write.
//   - @return: true on success, false on error.
bool add_io_event(sock_t fd, _ev_t ev);

// remove an IO event from epoll
//   - It MUST be called in coroutine.
void del_io_event(sock_t fd, _ev_t ev);

// remove all IO events on the socket
//   - It MUST be called in coroutine.
void del_io_event(sock_t fd);

// suspend the current coroutine
//   - It MUST be called in coroutine.
//   - Usually, users should add an IO event, or a timer, or both in a
//   coroutine,
//     and then call yield() to suspend the coroutine. When the event is present
//     or the timer expires, the scheduler will resume the coroutine.
void yield();

// resume the coroutine
//   - It is thread safe and can be called anywhere.
//   - @co: a pointer to the coroutine (result of co::coroutine())
void resume(void *co);

// sleep for milliseconds
//   - It is equal to sleep::ms() if called from non-coroutines.
void sleep(uint32_t ms);

// check whether the current coroutine has timed out
//   - It MUST be called in coroutine.
//   - When a coroutine returns from an API with a timeout like co::recv, users
//   may
//     call co::timeout() to check whether the API call has timed out.
bool timeout();

// check whether a pointer is on the stack of the current coroutine
//   - It MUST be called in coroutine.
bool on_stack(const void *p);

// stop all schedulers
void stop_scheds();

} // namespace co
} // namespace coost
using coost::co::go;
