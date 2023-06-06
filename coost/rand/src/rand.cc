#include <coost/macro.h>
#include <coost/rand.h>
#include <coost/god.h>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <cstring>

#ifdef _WIN32
#include <intrin.h>

inline uint32_t _get_mask(uint32_t x) { /* x > 1 */
  unsigned long r;
  _BitScanReverse(&r, x - 1);
  return (2u << r) - 1;
}

#else
inline uint32_t _get_mask(uint32_t x) { /* x > 1 */
  return (2u << (31 - __builtin_clz(x - 1))) - 1;
}
#endif

namespace coost {

class Rand {
public:
  Rand() : _mt(std::random_device{}()) {
    const uint32_t seed = _mt();
    _seed = (0 < seed && seed < 2147483647u) ? seed : 23u;
  }

  // _seed = _seed * A % M
  uint32_t next() {
    static const uint32_t M = 2147483647u; // 2^31-1
    static const uint64_t A = 16385;       // 2^14+1
    const uint64_t p = _seed * A;
    _seed = static_cast<uint32_t>((p >> 31) + (p & M));
    return _seed > M ? (_seed -= M) : _seed;
  }

  std::mt19937 &mt19937() { return _mt; }

private:
  std::mt19937 _mt;
  uint32_t _seed;
};

inline Rand &_rand() {
  static thread_local Rand r;
  return r;
}

uint32_t rand() { return _rand().next(); }

inline void _gen_random_bytes(uint8_t *p, uint32_t n) {
  auto &r = _rand().mt19937();
  const uint32_t x = (n >> 2) << 2;
  uint32_t i = 0;
  for (; i < x; i += 4)
    *(uint32_t *)(p + i) = r();
  i < n ? (void)(*(uint32_t *)(p + i) = r()) : (void)0;
}

const char kS[] =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";

std::vector<uint8_t> rand_bytes(int n) {
  std::vector<uint8_t> bytes(n);
  _gen_random_bytes((uint8_t *)bytes.data(), n);
  return bytes;
}

uint64_t rand_u64_id() {
  union {
    uint8_t bytes[8];
    uint64_t v;
  } u;
  _gen_random_bytes(u.bytes, 8);
  return u.v;
}

std::string rand_short_id() { return to_short_id(rand_u64_id()); }

uint8_t char64_to_u8(const std::string &short_id, int i) {
  if (i < 0)
    return 0;
  auto c = short_id[i];
  uint8_t v;
  if (c == '-')
    v = 63;
  else if (c == '_')
    v = 62;
  else if (c >= 0x61)
    v = c - 'a' + 10;
  else if (c >= 0x41)
    v = c - 'A' + 36;
  else
    v = c - '0';
  return v;
}

uint64_t to_u64_id(const std::string &short_id) {
  auto size = short_id.size();
  auto step = size / 4;
  step += !!(size - step * 4);

  uint64_t id = 0;
  auto data = (uint8_t *)&id;

  const auto last_idx = size - 1;
  for (int idx = 0; idx < step; ++idx) {
    auto offset = last_idx - idx * 4;
    auto v0 = char64_to_u8(short_id, offset);
    auto v1 = char64_to_u8(short_id, offset - 1);
    auto v2 = char64_to_u8(short_id, offset - 2);
    auto v3 = char64_to_u8(short_id, offset - 3);
    auto i = idx * 3;
    data[i] = v0 | ((v1 & 0b11) << 6);
    data[i + 1] = (v1 >> 2) | ((v2 & 0b1111) << 4);
    if (idx < 2)
      data[i + 2] = (v2 >> 4) | (v3 << 2);
  }
  return id;
}

std::string to_short_id(uint64_t id) {
  auto data = (uint8_t *)&id;
  std::vector<char> v;
  auto d0 = *(data + 7);
  auto v0 = d0 >> 4;
  if (v0)
    v.push_back(kS[v0]);
  auto d1 = *(data + 6);
  auto v1 = ((d0 & 0b1111) << 2) | (d1 >> 6);
  if (v1 || v.size())
    v.push_back(kS[v1]);
  auto v2 = d1 & 0b111111;
  if (v2 || v.size())
    v.push_back(kS[v2]);
  for (int i = 0; i < 2; ++i) {
    auto o = i * 3;
    auto x0 = *(data + 5 - o);
    auto x1 = *(data + 4 - o);
    auto x2 = *(data + 3 - o);
    auto v1 = x0 >> 2;
    if (v1 || v.size())
      v.push_back(kS[v1]);
    auto v2 = ((x0 & 0b11) << 4) | (x1 >> 4);
    if (v2 || v.size())
      v.push_back(kS[v2]);
    auto v3 = ((x1 & 0b1111) << 2) | (x2 >> 6);
    if (v3 || v.size())
      v.push_back(kS[v3]);
    auto v4 = x2 & 0b111111;
    if (v4 || v.size())
      v.push_back(kS[v4]);
  }
  return std::string(v.begin(), v.end());
}

// A thread-safe C++ implement for nanoid
// Inspired by github.com/mcmikecreations/nanoid_cpp.
// Also see https://github.com/ai/nanoid for details.
std::string randstr(int n) {
  n <= 0 ? (void)(n = 15) : (void)0;
  const uint32_t mask = 63;
  const uint32_t p = mask * n;
  const uint32_t r = p / 40;
  const uint32_t step = r + !!(p - r * 40); // ceil(1.6 * mask * n / 64)
  std::vector<uint8_t> bytes(god::align_up<4>(step));
  std::vector<uint8_t> res(n);
  int pos = 0;

  res.resize(n);
  while (true) {
    std::cout << "_gen_random_bytes " << step << std::endl;
    _gen_random_bytes((uint8_t *)bytes.data(), step);
    for (size_t i = 0; i < step; ++i) {
      const size_t index = bytes[i] & mask;
      res[pos] = kS[index];
      if (++pos == n)
        return std::string(res.begin(), res.end());
    }
  }
}

// 2 <= len <= 255, n > 0
std::string randstr(const char *s, int n) {
  const uint32_t len = (uint32_t)strlen(s);
  if (unlikely(len < 2 || len > 255))
    return std::string();
  n <= 0 ? (void)(n = 15) : (void)0;

  const uint32_t mask = _get_mask(len);
  const uint32_t step = (uint32_t)::ceil(1.6 * (mask * n) / len);
  std::vector<uint8_t> bytes(god::align_up<4>(step));
  std::vector<uint8_t> res(n);
  int pos = 0;

  res.resize(n);
  while (true) {
    _gen_random_bytes((uint8_t *)bytes.data(), step);
    for (size_t i = 0; i < step; ++i) {
      const size_t index = bytes[i] & mask;
      if (index < len) {
        res[pos] = s[index];
        if (++pos == n)
          return std::string(res.begin(), res.end());
      }
    }
  }
}

} // namespace coost
