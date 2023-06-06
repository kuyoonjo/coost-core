#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace coost {

// generate a random number (0 < result < 2^31-1, thread-safe)
uint32_t rand();

// generate a random number with specific seed
// - @seed: 0 < seed < 2^31-1, initialize it with co::rand() for simplicity
inline uint32_t rand(uint32_t &seed) {
  static const uint32_t M = 2147483647u; // 2^31-1
  static const uint64_t A = 16385;       // 2^14+1
  const uint64_t p = seed * A;
  seed = static_cast<uint32_t>((p >> 31) + (p & M));
  return seed > M ? (seed -= M) : seed;
}

// return a random string with default symbols (thread-safe)
// - @n: length of the random string (>0), 15 by default
std::string randstr(int n = 15);

// return a random string with specific symbols (thread-safe)
// - @s: a null-terminated string stores the symbols to be used,
//       2 <= strlen(s) <= 255, otherwise return an empty string.
// - @n: length of the random string, n > 0
std::string randstr(const char *s, int n);

std::vector<uint8_t> rand_bytes(int n);
uint64_t rand_u64_id();
std::string rand_short_id();

std::string to_short_id(uint64_t id);
uint64_t to_u64_id(const std::string& short_id);

} // namespace coost
