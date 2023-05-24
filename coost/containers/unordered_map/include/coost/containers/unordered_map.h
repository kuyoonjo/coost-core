#pragma once

#include <coost/containers/compare.h>
#include <coost/mem.h>
#include <unordered_map>

namespace coost {
template <class K, class V, class Hash = compare::hash<K>,
          class Pred = compare::eq<K>,
          class Alloc = mem::stl_allocator<std::pair<const K, V>>>
using unordered_map = std::unordered_map<K, V, Hash, Pred, Alloc>;
} // namespace coost
