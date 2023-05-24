#pragma once

#include <coost/containers/compare.h>
#include <coost/mem.h>
#include <map>

namespace coost {

template <class K, class V, class L = compare::less<K>,
          class Alloc = mem::stl_allocator<std::pair<const K, V>>>
using map = std::map<K, V, L, Alloc>;

template <class K, class V, class L = compare::less<K>,
          class Alloc = mem::stl_allocator<std::pair<const K, V>>>
using multimap = std::multimap<K, V, L, Alloc>;
} // namespace coost
