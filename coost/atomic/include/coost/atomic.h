#pragma once

#include <coost/macro.h>

#ifdef _MSC_VER
#include <atomic>
#endif

namespace coost {

#ifdef _MSC_VER

using memory_order_t = std::memory_order;
constexpr memory_order_t mo_relaxed = std::memory_order_relaxed;
constexpr memory_order_t mo_consume = std::memory_order_consume;
constexpr memory_order_t mo_acquire = std::memory_order_acquire;
constexpr memory_order_t mo_release = std::memory_order_release;
constexpr memory_order_t mo_acq_rel = std::memory_order_acq_rel;
constexpr memory_order_t mo_seq_cst = std::memory_order_seq_cst;

// mo: mo_relaxed, mo_consume, mo_acquire, mo_seq_cst
template <typename T>
inline T atomic_load(const T *p, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->load(mo);
}

// mo: mo_relaxed, mo_release, mo_seq_cst
template <typename T, typename V>
inline void atomic_store(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  ((std::atomic<T> *)p)->store((T)v, mo);
}

// mo: all
template <typename T, typename V>
inline T atomic_swap(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->exchange((T)v, mo);
}

// smo: success memory order, all
// fmo: failure memory order, cannot be mo_release, mo_acq_rel,
//      and cannot be stronger than smo
template <typename T, typename O, typename V>
inline T atomic_compare_swap(T *p, O o, V v, memory_order_t smo = mo_seq_cst,
                             memory_order_t fmo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  T x = (T)o;
  ((std::atomic<T> *)p)->compare_exchange_strong(x, (T)v, smo, fmo);
  return x;
}

template <typename T, typename V>
inline T atomic_add(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_add(v, mo) + v;
}

template <typename T, typename V>
inline T atomic_sub(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_sub(v, mo) - v;
}

template <typename T>
inline T atomic_inc(T *p, memory_order_t mo = mo_seq_cst) {
  return atomic_add(p, 1, mo);
}

template <typename T>
inline T atomic_dec(T *p, memory_order_t mo = mo_seq_cst) {
  return atomic_sub(p, 1, mo);
}

template <typename T, typename V>
inline T atomic_fetch_add(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_add(v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_sub(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_sub(v, mo);
}

template <typename T>
inline T atomic_fetch_inc(T *p, memory_order_t mo = mo_seq_cst) {
  return atomic_fetch_add(p, 1, mo);
}

template <typename T>
inline T atomic_fetch_dec(T *p, memory_order_t mo = mo_seq_cst) {
  return atomic_fetch_sub(p, 1, mo);
}

template <typename T, typename V>
inline T atomic_or(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_or((T)v, mo) | (T)v;
}

template <typename T, typename V>
inline T atomic_and(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_and((T)v, mo) & (T)v;
}

template <typename T, typename V>
inline T atomic_xor(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_xor((T)v, mo) ^ (T)v;
}

template <typename T, typename V>
inline T atomic_fetch_or(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_or((T)v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_and(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_and((T)v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_xor(T *p, V v, memory_order_t mo = mo_seq_cst) {
  static_assert(sizeof(T) == sizeof(std::atomic<T>), "");
  return ((std::atomic<T> *)p)->fetch_xor((T)v, mo);
}

#else /* gcc/clang */
//     |
//     v   ^
//---------|-----  release barrier
//
//---------|-----  acquire barrier
//     ^   v
//     |
enum memory_order_t {
  mo_relaxed = __ATOMIC_RELAXED,
  mo_consume = __ATOMIC_CONSUME,
  mo_acquire = __ATOMIC_ACQUIRE,
  mo_release = __ATOMIC_RELEASE,
  mo_acq_rel = __ATOMIC_ACQ_REL,
  mo_seq_cst = __ATOMIC_SEQ_CST,
};

// mo: mo_relaxed, mo_consume, mo_acquire, mo_seq_cst
template <typename T>
inline T atomic_load(T *p, memory_order_t mo = mo_seq_cst) {
  return __atomic_load_n(p, mo);
}

// mo: mo_relaxed, mo_release, mo_seq_cst
template <typename T, typename V>
inline void atomic_store(T *p, V v, memory_order_t mo = mo_seq_cst) {
  __atomic_store_n(p, (T)v, mo);
}

// mo: all
template <typename T, typename V>
inline T atomic_swap(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_exchange_n(p, (T)v, mo);
}

// smo: success memory order, all
// fmo: failure memory order, cannot be mo_release, mo_acq_rel,
//      and cannot be stronger than smo
template <typename T, typename O, typename V>
inline T atomic_compare_swap(T *p, O o, V v, memory_order_t smo = mo_seq_cst,
                             memory_order_t fmo = mo_seq_cst) {
  T x = (T)o;
  __atomic_compare_exchange_n(p, &x, (T)v, false, smo, fmo);
  return x;
}

template <typename T>
inline T atomic_inc(T *p, memory_order_t mo = mo_seq_cst) {
  return __atomic_add_fetch(p, 1, mo);
}

template <typename T>
inline T atomic_dec(T *p, memory_order_t mo = mo_seq_cst) {
  return __atomic_sub_fetch(p, 1, mo);
}

template <typename T, typename V>
inline T atomic_add(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_add_fetch(p, v, mo);
}

template <typename T, typename V>
inline T atomic_sub(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_sub_fetch(p, v, mo);
}

template <typename T>
inline T atomic_fetch_inc(T *p, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_add(p, 1, mo);
}

template <typename T>
inline T atomic_fetch_dec(T *p, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_sub(p, 1, mo);
}

template <typename T, typename V>
inline T atomic_fetch_add(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_add(p, v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_sub(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_sub(p, v, mo);
}

template <typename T, typename V>
inline T atomic_or(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_or_fetch(p, v, mo);
}

template <typename T, typename V>
inline T atomic_and(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_and_fetch(p, v, mo);
}

template <typename T, typename V>
inline T atomic_xor(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_xor_fetch(p, v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_or(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_or(p, v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_and(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_and(p, v, mo);
}

template <typename T, typename V>
inline T atomic_fetch_xor(T *p, V v, memory_order_t mo = mo_seq_cst) {
  return __atomic_fetch_xor(p, v, mo);
}

#endif

// the same as atomic_compare_swap
// smo: success memory order, all
// fmo: failure memory order, cannot be mo_release, mo_acq_rel,
//      and cannot be stronger than smo
template <typename T, typename O, typename V>
inline T atomic_cas(T *p, O o, V v, memory_order_t smo = mo_seq_cst,
                    memory_order_t fmo = mo_seq_cst) {
  return atomic_compare_swap(p, o, v, smo, fmo);
}

// like the atomic_cas, but return true if the swap operation is successful
// smo: success memory order, all
// fmo: failure memory order, cannot be mo_release, mo_acq_rel,
//      and cannot be stronger than smo
template <typename T, typename O, typename V>
inline bool atomic_bool_cas(T *p, O o, V v, memory_order_t smo = mo_seq_cst,
                            memory_order_t fmo = mo_seq_cst) {
  return atomic_cas(p, o, v, smo, fmo) == (T)o;
}

} // namespace coost

#define COOST_VAR_COUNTER PP_CONCAT(_coost_var_counter_, __LINE__)

#define COOST_ATOMIC_FIRST(n)                                                  \
  static int COOST_VAR_COUNTER = 0;                                            \
  if (COOST_VAR_COUNTER < (n) &&                                               \
      coost::atomic_fetch_inc(&COOST_VAR_COUNTER, coost::mo_relaxed) < (n))

#define COOST_ATOMIC_EVERY(n)                                                  \
  static unsigned int COOST_VAR_COUNTER = 0;                                   \
  if (coost::atomic_fetch_inc(&COOST_VAR_COUNTER, coost::mo_relaxed) % (n) == 0)
