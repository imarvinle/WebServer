/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include "../include/socket.h"

#include <cstdio>
#include <cstring>

#include "../include/util.h"

namespace csguide_webserver {

void SetReusePort(int fd) {
  int opt = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
}

ServerSocket::ServerSocket(int port, const char *ip) : port_(port), ip_(ip) {
  bzero(&sockaddr_in_, sizeof(sockaddr_in_));
    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_port = htons(port);
  if (ip != nullptr) {
    ::inet_pton(AF_INET, ip, &sockaddr_in_.sin_addr);
  } else {
      sockaddr_in_.sin_addr.s_addr = htonl(INADDR_ANY);
  }
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd_ == -1) {
    std::cout << "creat socket error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
  SetReusePort(listen_fd_);
  SetNonBlocking(listen_fd_);  // FIXME 之前没加，导致循环接受阻塞了
}

void ServerSocket::Bind() {
  int ret = ::bind(listen_fd_, (struct sockaddr *)&sockaddr_in_, sizeof(sockaddr_in_));
  if (ret == -1) {
    std::cout << "bind error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
}

void ServerSocket::Listen() {
  int ret = ::listen(listen_fd_, 1024);
  if (ret == -1) {
    std::cout << "listen error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    exit(0);
  }
}

int ServerSocket::Accept(ClientSocket & client_socket) const {
  // std::cout << "listen_fd" << listen_fd << std::endl;
  // FIXME 之前这里出报 Invalid arg
  // int clientfd = ::accept(listen_fd, (struct sockaddr*)&clientSocket.mAddr,
  // &clientSocket.mLen);
  int clientfd = ::accept(listen_fd_, NULL, NULL);

  // FIXME 由于之前listen_fd是阻塞的逻辑正确，但是对于非阻塞listen_fd不正确
  //    if (clientfd < 0) {
  //        std::cout << "accept error in file <" << __FILE__ << "> "<< "at " <<
  //        __LINE__ << std::endl; exit(0);
  //    }
  if (clientfd < 0) {
    if ((errno == EWOULDBLOCK) || (errno == EAGAIN)) return clientfd;
    std::cout << "accept error in file <" << __FILE__ << "> "
              << "at " << __LINE__ << std::endl;
    std::cout << "clientfd:" << clientfd << std::endl;
    perror("accpet error");
    // exit(0);
  }
  // std::cout << "accept a client： " << clientfd << std::endl;
  client_socket.fd_ = clientfd;
  return clientfd;
}

void ServerSocket::Close() {
  if (listen_fd_ >= 0) {
    ::close(listen_fd_);
    // std::cout << "定时器超时关闭, 文件描述符:" << listen_fd << std::endl;
    listen_fd_ = -1;
  }
}
ServerSocket::~ServerSocket() { Close(); }

void ClientSocket::close() {
  if (fd_ >= 0) {
    // std::cout << "文件描述符关闭: " << fd <<std::endl;
    ::close(fd_);
      fd_ = -1;
  }
}
ClientSocket::~ClientSocket() { close(); }

}  // namespace csguide_webserver