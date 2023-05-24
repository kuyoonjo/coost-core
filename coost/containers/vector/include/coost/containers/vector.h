#pragma once

#include <coost/mem.h>
#include <vector>

namespace coost {
template <class T, class Alloc = coost::mem::stl_allocator<T>>
using vector = std::vector<T, Alloc>;
}
