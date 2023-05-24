#pragma once

#include <stdint.h>
#include <stddef.h>

#define MAX_UINT8  ((uint8_t)  ~((uint8_t) 0))
#define MAX_UINT16 ((uint16_t) ~((uint16_t)0))
#define MAX_UINT32 ((uint32_t) ~((uint32_t)0))
#define MAX_UINT64 ((uint64_t) ~((uint64_t)0))
#define MAX_INT8   ((int8_t)  (MAX_UINT8  >> 1))
#define MAX_INT16  ((int16_t) (MAX_UINT16 >> 1))
#define MAX_INT32  ((int32_t) (MAX_UINT32 >> 1))
#define MAX_INT64  ((int64_t) (MAX_UINT64 >> 1))
#define MIN_INT8   ((int8_t)  ~MAX_INT8)
#define MIN_INT16  ((int16_t) ~MAX_INT16)
#define MIN_INT32  ((int32_t) ~MAX_INT32)
#define MIN_INT64  ((int64_t) ~MAX_INT64)

#define DISALLOW_COPY_AND_ASSIGN(T) \
    T(const T&) = delete; \
    void operator=(const T&) = delete

#if SIZE_MAX == UINT64_MAX
#define __arch64 1
#else
#define __arch32 1
#endif

#ifdef _MSC_VER
#ifndef __thread
#define __thread __declspec(thread)
#endif
#else
#ifndef __forceinline 
#define __forceinline __attribute__((always_inline))
#endif
#endif

#ifndef unlikely
#if (defined(__GNUC__) && __GNUC__ >= 3) || defined(__clang__)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define unlikely(x) (x)
#endif
#endif

#ifndef PP_STRIFY
#define _PP_STRIFY(x) #x
#define PP_STRIFY(x) _PP_STRIFY(x)
#endif

#ifndef PP_CONCAT
#define _PP_CONCAT(x, y) x##y
#define PP_CONCAT(x, y) _PP_CONCAT(x, y)
#endif
