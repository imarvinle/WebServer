/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#ifndef WEBSERVER_SOCKET_H
#define WEBSERVER_SOCKET_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

namespace nsocket {
class ClientSocket;

void setReusePort(int fd);

class ServerSocket {
public:
  ServerSocket(int port = 8080, const char *ip = nullptr);
  ~ServerSocket();
  void bind();

  void listen();

  int accept(ClientSocket &);

public:
  sockaddr_in mAddr;
  int fd;
  int mPort;
  const char *mIp;
};

class ClientSocket {
public:
  ~ClientSocket();

  socklen_t mLen;
  sockaddr_in mAddr;
  int fd;
};
}  // namespace nsocket
#endif  // WEBSERVER_SOCKET_H
