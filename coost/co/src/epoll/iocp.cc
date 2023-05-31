#ifdef _WIN32

#include "iocp.h"

namespace coost {
namespace co {

Iocp::Iocp(int sched_id) : _signaled(0), _sched_id(sched_id) {
  _iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 1);
  if(_iocp == 0) COOST_LOG_FATAL( "create iocp failed..");
  _ev = (OVERLAPPED_ENTRY *)::calloc(1024, sizeof(OVERLAPPED_ENTRY));
}

Iocp::~Iocp() {
  if (_iocp) {
    CloseHandle(_iocp);
    _iocp = 0;
  }
  ::free(_ev);
}

} // namespace co
} // namespace coost

#endif
