#pragma once

#include <coost/containers/compare.h>
#include <coost/mem.h>
#include <set>

namespace coost {

template <class K, class L = compare::less<K>,
          class Alloc = mem::stl_allocator<K>>
using set = std::set<K, L, Alloc>;

template <class K, class L = compare::less<K>,
          class Alloc = mem::stl_allocator<K>>
using multiset = std::multiset<K, L, Alloc>;
} // namespace coost
