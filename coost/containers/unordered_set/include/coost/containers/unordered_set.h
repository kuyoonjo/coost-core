#pragma once

#include <coost/containers/compare.h>
#include <coost/mem.h>
#include <unordered_set>

namespace coost {
template <class K, class Hash = compare::hash<K>, class Pred = compare::eq<K>,
          class Alloc = mem::stl_allocator<K>>
using unordered_set = std::unordered_set<K, Hash, Pred, Alloc>;
} // namespace coost
