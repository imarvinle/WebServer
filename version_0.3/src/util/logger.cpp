/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: 简单的日志类
 */

#include "../../include/logger.h"

namespace csguide_webserver {

Logger::Logger() : use_file_(false) {}

Logger::~Logger() {
  if (use_file_) {
    log_file_.close();
  }
}

void Logger::SetLogFile(const std::string& log_file_path) {
  log_file_.open(log_file_path, std::ofstream::out | std::ofstream::app);
  use_file_ = log_file_.is_open();
  if (!use_file_) {
    std::cerr << "Failed to open log file: " << log_file_path << std::endl;
  }
}

const char* Logger::LogLevelToString(LogLevel level) {
  switch (level) {
    case LogLevel::RUN:
      return "RUN";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::DEBUG:
      return "DEBUG";
    default:
      break;
  }
  return "";
}


std::string Logger::FormatMessage(const std::string& fmt_str, ...) {
    va_list args;

    va_start(args, fmt_str);
    int buffer_size = std::vsnprintf(NULL, 0, fmt_str.c_str(), args) + 1;
    va_end(args);
    
    // 使用new和delete 替换 std::make_unique
    char* buffer = new char[buffer_size];

    va_start(args, fmt_str);
    std::vsnprintf(buffer, buffer_size, fmt_str.c_str(), args);
    va_end(args);

    std::string formatted_message(buffer, buffer + buffer_size - 1);
    
    delete[] buffer;
    
    return formatted_message;
}



}  // namespace csguide_webserver