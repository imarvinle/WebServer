/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <memory>

#include "HttpParse.h"
#include "HttpResponse.h"
#include "Socket.h"
#include "Timer.h"

namespace csguide_webserver {

class TimerNode;

class HttpData : public std::enable_shared_from_this<HttpData> {
  public:
  HttpData() : epoll_fd(-1) {}

  public:
  std::shared_ptr<HttpRequest> request_;
  std::shared_ptr<HttpResponse> response_;
  std::shared_ptr<ClientSocket> clientSocket_;
  int epoll_fd;

  void closeTimer();

  void setTimer(std::shared_ptr<TimerNode>);

  private:
  std::weak_ptr<TimerNode> timer_;
};

}  // namespace csguide_webserver