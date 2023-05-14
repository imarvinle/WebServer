/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <memory>

namespace csguide_webserver {

class ClientSocket;

void setReusePort(int fd);

class ServerSocket {
public:
  ServerSocket(int port = 8080, const char *ip = nullptr);

  ~ServerSocket();

  void Bind();

  void Listen();

  void Close();

  int Accept(ClientSocket &) const;

public:
  sockaddr_in sockaddr_in_;
  int listen_fd_;
  int epoll_fd_;
  int port_;
  const char *ip_;
};

class ClientSocket {
public:
  ClientSocket() { fd_ = -1; };

  void close();

  ~ClientSocket();

  socklen_t socklen_;
  sockaddr_in sockaddr_in_;
  int fd_;
};
}  // namespace csguide_webserver