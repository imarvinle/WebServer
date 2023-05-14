/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include "../include/util.h"

#include <signal.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstring>
#include <string>

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

int CheckBasePath(char *base_path) {
  struct stat file;
  if (stat(base_path, &file) == -1) {
    return -1;
  }
  // 不是目录 或者不可访问
  if (!S_ISDIR(file.st_mode) || access(base_path, R_OK) == -1) {
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

}  // namespace csguide_webserver