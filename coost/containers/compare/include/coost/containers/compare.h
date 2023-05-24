#pragma once

#include <coost/crypto/murmur_hash.h>
#include <cstring>
#include <functional>


namespace coost {
namespace compare {

template <class T> struct less {
  template <class X, class Y> bool operator()(X &&x, Y &&y) const {
    return static_cast<X &&>(x) < static_cast<Y &&>(y);
  }
};

template <> struct less<const char *> {
  bool operator()(const char *x, const char *y) const {
    return x != y && strcmp(x, y) < 0;
  }
};

template <class T> struct hash {
  size_t operator()(const T &x) const noexcept { return std::hash<T>()(x); }
};

template <> struct hash<const char *> {
  size_t operator()(const char *x) const noexcept {
    return coost::crypto::murmur_hash(x, strlen(x));
  }
};

template <class T> struct eq {
  template <class X, class Y> bool operator()(X &&x, Y &&y) const {
    return static_cast<X &&>(x) == static_cast<Y &&>(y);
  }
};

template <> struct eq<const char *> {
  bool operator()(const char *x, const char *y) const {
    return x == y || strcmp(x, y) == 0;
  }
};

} // namespace compare

} // namespace coost
