#pragma once

#include <assert.h>
#include <coost/macro.h>
#include <mutex>
#include <thread>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <pthread.h>
#endif

// add std::mutex_guard
namespace std {
typedef std::lock_guard<std::mutex> mutex_guard;
} // namespace std

namespace coost {
namespace co {
namespace xx {
uint32_t thread_id();

#ifdef _WIN32
typedef DWORD tls_key_t;
inline void tls_init(tls_key_t *k) {
  *k = TlsAlloc();
  assert(*k != TLS_OUT_OF_INDEXES);
}
inline void tls_free(tls_key_t k) { TlsFree(k); }
inline void *tls_get(tls_key_t k) { return TlsGetValue(k); }
inline void tls_set(tls_key_t k, void *v) {
  BOOL r = TlsSetValue(k, v);
  assert(r);
  (void)r;
}

#else
typedef pthread_key_t tls_key_t;
inline void tls_init(tls_key_t *k) {
  int r = pthread_key_create(k, 0);
  (void)r;
  assert(r == 0);
}
inline void tls_free(tls_key_t k) {
  int r = pthread_key_delete(k);
  (void)r;
  assert(r == 0);
}
inline void *tls_get(tls_key_t k) { return pthread_getspecific(k); }
inline void tls_set(tls_key_t k, void *v) {
  int r = pthread_setspecific(k, v);
  (void)r;
  assert(r == 0);
}
#endif
} // namespace xx

// get id of the current thread
inline uint32_t thread_id() {
  static __thread uint32_t id = 0;
  return id != 0 ? id : (id = xx::thread_id());
}

template <typename T> class tls {
public:
  tls() { xx::tls_init(&_key); }
  ~tls() { xx::tls_free(_key); }

  T *get() const { return (T *)xx::tls_get(_key); }

  void set(T *p) { xx::tls_set(_key, p); }

  T *operator->() const {
    T *const o = this->get();
    assert(o);
    return o;
  }

  T &operator*() const {
    T *const o = this->get();
    assert(o);
    return *o;
  }

  bool operator==(T *p) const { return this->get() == p; }
  bool operator!=(T *p) const { return this->get() != p; }
  explicit operator bool() const { return this->get() != 0; }

private:
  xx::tls_key_t _key;
  DISALLOW_COPY_AND_ASSIGN(tls);
};

// for threads only
class sync_event {
public:
  explicit sync_event(bool manual_reset = false, bool signaled = false);
  ~sync_event();

  sync_event(sync_event &&e) noexcept : _p(e._p) { e._p = 0; }

  void signal();
  void reset();
  void wait();
  bool wait(uint32_t ms);

private:
  void *_p;
  DISALLOW_COPY_AND_ASSIGN(sync_event);
};

} // namespace co
} // namespace coost