/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <sys/epoll.h>

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "http_data.h"
#include "socket.h"
#include "timer.h"

namespace csguide_webserver {

class Epoll {
  
public:
  static int Init(int max_events);

  static int Addfd(int epoll_fd, int fd, __uint32_t events, std::shared_ptr<HttpData>);

  static int Modfd(int epoll_fd, int fd, __uint32_t events, std::shared_ptr<HttpData>);

  static int Delfd(int epoll_fd, int fd, __uint32_t events);

  static std::vector<std::shared_ptr<HttpData>> Poll(const ServerSocket &serverSocket, int max_event, int timeout);

  static void HandleConnection(const ServerSocket &serverSocket);

public:
  static std::unordered_map<int, std::shared_ptr<HttpData>> http_data_map_;
  static const int MAX_EVENTS;
  static epoll_event *events_;
  static TimerManager timer_manager_;
  const static __uint32_t DEFAULT_EVENTS;
};
}  // namespace csguide_webserver