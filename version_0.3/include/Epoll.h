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

#include "HttpData.h"
#include "Socket.h"
#include "Timer.h"

class Epoll {
  public:
  static int init(int max_events);

  static int addfd(int epoll_fd, int fd, __uint32_t events,
                   std::shared_ptr<HttpData>);

  static int modfd(int epoll_fd, int fd, __uint32_t events,
                   std::shared_ptr<HttpData>);

  static int delfd(int epoll_fd, int fd, __uint32_t events);

  static std::vector<std::shared_ptr<HttpData>> poll(
      const ServerSocket &serverSocket, int max_event, int timeout);

  static void handleConnection(const ServerSocket &serverSocket);

  public:
  static std::unordered_map<int, std::shared_ptr<HttpData>> httpDataMap;
  static const int MAX_EVENTS;
  static epoll_event *events;
  static TimerManager timerManager;
  const static __uint32_t DEFAULT_EVENTS;
};

//#endif //WEBSERVER_EPOLL_H
