/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <optional>
#include <stdexcept>
#include <string>

#include "../../include/ini_file.h"
#include "../../include/logger.h"
#include "../../include/server.h"
#include "../../include/util.h"

namespace csguide_webserver {

std::string &Ltrim(std::string &str) {
  if (str.empty()) {
    return str;
  }

  str.erase(0, str.find_first_not_of(" \t"));
  return str;
}

std::string &Rtrim(std::string &str) {
  if (str.empty()) {
    return str;
  }
  str.erase(str.find_last_not_of(" \t") + 1);
  return str;
}

std::string &Trim(std::string &str) {
  if (str.empty()) {
    return str;
  }

  Ltrim(str);
  Rtrim(str);
  return str;
}

int SetNonBlocking(int fd) {
  int old_option = fcntl(fd, F_GETFL);
  int new_option = old_option | O_NONBLOCK;
  fcntl(fd, F_SETFL, new_option);
  return old_option;
}

void HandleForSigPipe() {
  struct sigaction sa;
  memset(&sa, '\0', sizeof(sa));
  sa.sa_handler = SIG_IGN;
  sa.sa_flags = 0;
  if (sigaction(SIGPIPE, &sa, NULL)) return;
}

int CheckBasePath(const std::string &base_path) {
  struct stat file;
  if (stat(base_path.c_str(), &file) == -1) {
    Logger::GetInstance().LogErr("path not exist: %s", base_path.c_str());
    return -1;
  }
  // 不是目录 或者不可访问
  if (!S_ISDIR(file.st_mode) || access(base_path.c_str(), R_OK) == -1) {
    Logger::GetInstance().LogErr("path not directory or can't access: %s", base_path.c_str());
    return -1;
  }
  return 0;
}

bool EndsWith(std::string const &str, std::string const &suffix) {
  if (str.length() < suffix.length()) {
    return false;
  }
  return str.rfind(suffix) == str.size() - suffix.size();
}

int ParseConfig(const std::string config_file, ServerConf &server_conf) {
  INIFile ini_file;
  int ret = 0;

  if (ini_file.Load(config_file)) {
    const INISection *worker_section = ini_file.GetSection("Worker");
    if (worker_section) {
      ret = StrToInt(worker_section->GetValue("thread_num"), *(&server_conf.thread_num));
      if (0 != ret) {
        return ret;
      }
      ret = StrToInt(worker_section->GetValue("port"), *(&server_conf.port));
      if (0 != ret) {
        return ret;
      }
      int daemon = 0;
      ret = StrToInt(worker_section->GetValue("daemon"), *(&daemon));
      if (0 != ret) {
        return ret;
      }
      server_conf.daemon = (daemon == 1);
    } else {
      Logger::GetInstance().LogErr("Worker Section not found, configfile=%s", config_file.c_str());
      return -1;
    }

    const INISection *server_section = ini_file.GetSection("Server");
    if (server_section) {
      server_conf.root = server_section->GetValue("root");
    } else {
      Logger::GetInstance().LogErr("Server Section not found, configfile=%s", config_file.c_str());
      return -1;
    }
  } else {
    // 配置读取失败，返回失败
    Logger::GetInstance().LogErr("Failed to load configfile: %s", config_file.c_str());
    return -1;
  }
  return 0;
}

int StrToInt(const std::string &str, int &result) {
  try {
    result = std::stoi(str);
    return 0;
  } catch (const std::invalid_argument &) {
    std::cerr << "非法的参数: " << str << '\n';
  } catch (const std::out_of_range &) {
    std::cerr << "超出范围: " << str << '\n';
  } catch (...) {
    std::cerr << "未知错误" << '\n';
  }
  return -1;  // 转换失败返回-1
}

std::string ExtractSubstring(const std::string &input, char start_char, char end_char) {
  size_t start = input.find(start_char);
  size_t end = input.rfind(end_char);

  if (start != std::string::npos && end != std::string::npos && end > start) {
    return input.substr(start + 1, end - start - 1);
  } else {
    return "";  // 返回空字符串，表示未找到子字符串
  }
}

}  // namespace csguide_webserver