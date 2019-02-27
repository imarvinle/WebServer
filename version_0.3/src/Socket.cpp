//
// Created by marvinle on 2019/2/1 4:21 PM.
//

#include "../include/Socket.h"


void setReusePort(int fd) {
    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt));
}

ServerSocket::ServerSocket(int port, const char *ip) : mPort(port), mIp(ip) {
    bzero(&mAddr, sizeof(mAddr));
    mAddr.sin_family = AF_INET;
    mAddr.sin_port = htons(port);
    if (ip != nullptr) {
        ::inet_pton(AF_INET, ip, &mAddr.sin_addr);
    } else {
        mAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1){
        std::cout << "creat socket error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        exit(0);
    }
    setReusePort(listen_fd);
}

void ServerSocket::bind() {
    int ret = ::bind(listen_fd, (struct sockaddr*)&mAddr, sizeof(mAddr));
    if (ret == -1) {
        std::cout << "bind error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        exit(0);
    }
}

void ServerSocket::listen() {
    int ret = ::listen(listen_fd, 5);
    if (ret == -1) {
        std::cout << "listen error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        exit(0);
    }
}

int ServerSocket::accept(ClientSocket &clientSocket) const {
    int clientfd = ::accept(listen_fd, (struct sockaddr*)&clientSocket.mAddr, &clientSocket.mLen);
    if (clientfd < 0) {
        std::cout << "accept error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        exit(0);
    }
    clientSocket.fd = clientfd;
    std::cout << "accept a client" << std::endl;
    return clientfd;
}

void ServerSocket::close() {
    if (listen_fd >= 0) {
        ::close(listen_fd);
        listen_fd = -1;
    }
}
ServerSocket::~ServerSocket() {
    close();
}

void ClientSocket::close() {
    if (fd >= 0) {
        ::close(fd);
        fd = -1;
    }
}
ClientSocket::~ClientSocket() {
    close();
}



