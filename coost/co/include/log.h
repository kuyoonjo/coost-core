#pragma once

#ifdef CO_LOG_HEADER
#else
#include <coost/output.h>
#include <coost/fastream.h>
#include <chrono>

constexpr const char *labels[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR"};

namespace coost {
namespace log_level {
constexpr int TRACE = 0;
constexpr int DEBUG = 1;
constexpr int INFO = 2;
constexpr int WARN = 3;
constexpr int ERROR = 4;
}
template <int log_level, typename... X> inline void log(X &&...x) {
  auto now = std::chrono::system_clock::now();
  std::time_t dt =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  char buf[32] = {0};
  std::strftime(buf, sizeof(buf), "%y-%m-%d %T.", std::localtime(&dt));

  auto seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
  auto fraction = now - seconds;
  auto milliseconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(fraction);
  coost::fastream fs;
  fs << '[' << buf << milliseconds.count() << ' ' << labels[log_level] << "] ";
  coost::println(fs.c_str(), x...);
}
} // namespace coost
#define CO_LOG_TRACE coost::log<coost::log_level::TRACE>
#define CO_LOG_DEBUG coost::log<coost::log_level::DEBUG>
#define CO_LOG_INFO coost::log<coost::log_level::INFO>
#define CO_LOG_WARN coost::log<coost::log_level::WARN>
#define CO_LOG_ERROR coost::log<coost::log_level::ERROR>
#endif