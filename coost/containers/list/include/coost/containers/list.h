#pragma once

#include <coost/mem.h>
#include <list>

namespace coost {
template <class T, class Alloc = coost::mem::stl_allocator<T>>
using list = std::list<T, Alloc>;
}
