#pragma once

#include <coost/macro.h>

#ifndef _WIN32
#include <errno.h>
#endif

namespace coost {

#ifdef _WIN32
int &error();
#else
inline int &error() { return errno; }
#endif

// get string of a error number (thread-safe)
const char *strerror(int e);

// get string of the current error number (thread-safe)
inline const char *strerror() { return strerror(error()); }

} // namespace coost