#pragma once

#include <coost/output.h>
#include <coost/fastream.h>
#include <chrono>
#include <cstdlib>

constexpr const char *labels[] = {"TRACE", "DEBUG", "INFO",
                                  "WARN",  "ERROR", "FATAL"};

namespace coost {
enum log_level {
  Trace = 0,
  Debug = 1,
  Info = 2,
  Warn = 3,
  Error = 4,
  Fatal = 5,
};
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
  fs << "- " << '[' << buf << milliseconds.count() << ' ' << labels[log_level] << "] ";
  coost::println(fs.c_str(), x...);
  if constexpr (log_level == log_level::Fatal) {
    abort();
  }
}
} // namespace coost
#define COOST_LOG_TRACE coost::log<coost::log_level::Trace>
#define COOST_LOG_DEBUG coost::log<coost::log_level::Debug>
#define COOST_LOG_INFO coost::log<coost::log_level::Info>
#define COOST_LOG_WARN coost::log<coost::log_level::Warn>
#define COOST_LOG_ERROR coost::log<coost::log_level::Error>
#define COOST_LOG_FATAL coost::log<coost::log_level::Fatal>