/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */
#include "../include/ssocket.h"

void nsocket::setReusePort(int fd) {
  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
}

nsocket::ServerSocket::ServerSocket(int port, const char *ip) : mPort(port), mIp(ip) {
  bzero(&mAddr, sizeof(mAddr));
  mAddr.sin_family = AF_INET;
  mAddr.sin_port = htons(port);
  if (ip != nullptr) {
    ::inet_pton(AF_INET, ip, &mAddr.sin_addr);
  } else {
    mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  }
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    std::cout << "creat socket error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
  setReusePort(fd);
}

void nsocket::ServerSocket::bind() {
  int ret = ::bind(fd, (struct sockaddr *)&mAddr, sizeof(mAddr));
  if (ret == -1) {
    std::cout << "bind error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
}

void nsocket::ServerSocket::listen() {
  int ret = ::listen(fd, 5);
  if (ret == -1) {
    std::cout << "listen error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
}

int nsocket::ServerSocket::accept(ClientSocket &clientSocket) {
  int clientfd = ::accept(fd, (struct sockaddr *)&clientSocket.mAddr, &clientSocket.mLen);
  if (clientfd < 0) {
    std::cout << "accept error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
  clientSocket.fd = clientfd;
  std::cout << "accept a client" << std::endl;
  return clientfd;
}

nsocket::ServerSocket::~ServerSocket() { ::close(fd); }

nsocket::ClientSocket::~ClientSocket() { ::close(fd); }
