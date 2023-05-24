#pragma once

#include <coost/mem.h>
#include <deque>

namespace coost {
template <class T, class Alloc = coost::mem::stl_allocator<T>>
using list = std::deque<T, Alloc>;
}
