/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: 简单的日志类
 */

#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace csguide_webserver {
#pragma once

#include <cstdarg>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

enum LogLevel { RUN, ERROR, DEBUG };

class Logger {
public:
  static Logger& GetInstance() {
    static Logger instance;
    return instance;
  }

  Logger(const Logger&) = delete;
  void operator=(const Logger&) = delete;

  void SetLogFile(const std::string& log_file_path);

  template <typename... Args>
  void Log(LogLevel level, const std::string& fmt_str, Args... args);

  template <typename... Args>
  void LogRun(const std::string& fmt_str, Args... args) {
    Log(RUN, fmt_str, args...);
  }

  template <typename... Args>
  void LogErr(const std::string& fmt_str, Args... args) {
    Log(ERROR, fmt_str, args...);
  }

  template <typename... Args>
  void LogDebug(const std::string& fmt_str, Args... args) {
    Log(DEBUG, fmt_str, args...);
  }

private:
  Logger();
  ~Logger();

  const char* LogLevelToString(LogLevel level);
  std::string FormatMessage(const std::string& fmt_str, ...);

  std::ofstream log_file_;
  bool use_file_;
};

// 在头文件中提供模板的定义以避免链接错误
template <typename... Args>
void Logger::Log(LogLevel level, const std::string& fmt_str, Args... args) {
  std::string message = FormatMessage(fmt_str.c_str(), args...);
  std::string formatted_msg = LogLevelToString(level) + std::string(": ") + message;

  if (use_file_) {
    log_file_ << formatted_msg << std::endl;
  } else {
    std::cout << formatted_msg << std::endl;
  }
}


}  // namespace csguide_webserver