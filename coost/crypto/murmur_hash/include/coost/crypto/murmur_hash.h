/*
 * Murmurhash from http://sites.google.com/site/murmurhash.
 * Written by Austin Appleby, and is placed to the public domain.
 * For business purposes, Murmurhash is under the MIT license.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <coost/macro.h>

namespace coost {
namespace crypto {

/**
 * 32 bit murmur hash
 *
 * @param s  a pointer to the data, it may not work on some systems if s is not
 *           4-byte aligned.
 * @param n  size of the data.
 */
uint32_t murmur_hash32(const void *s, size_t n, uint32_t seed);

/**
 * 64 bit murmur hash
 *
 * @param s  a pointer to the data, it may not work on some systems if s is not
 *           8-byte aligned.
 * @param n  size of the data.
 */
uint64_t murmur_hash64(const void *s, size_t n, uint64_t seed);

/**
 * platform-specific murmur hash
 *   - This function returns a hash value of type size_t, instead of uint32 or
 * uint64, which is to say, it is a 32 bit value on 32 bit platforms, or a 64
 * bit value on 64 bit platforms.
 */
#if __arch64
inline size_t murmur_hash(const void *s, size_t n, size_t seed = 0) {
  return murmur_hash64(s, n, seed);
}
#else
inline size_t murmur_hash(const void *s, size_t n, size_t seed = 0) {
  return murmur_hash32(s, n, seed);
}
#endif

} // namespace crypto
} // namespace coost