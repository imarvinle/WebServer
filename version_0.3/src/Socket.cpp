//
// Created by marvinle on 2019/2/1 4:21 PM.
//

#include "../include/Socket.h"
#include "../include/Util.h"
#include <cstring>
#include <cstdio>

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
    setnonblocking(listen_fd); // FIXME 之前没加，导致循环接受阻塞了
}

void ServerSocket::bind() {
    int ret = ::bind(listen_fd, (struct sockaddr*)&mAddr, sizeof(mAddr));
    if (ret == -1) {
        std::cout << "bind error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        exit(0);
    }
}

void ServerSocket::listen() {
    int ret = ::listen(listen_fd, 1024);
    if (ret == -1) {
        std::cout << "listen error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        exit(0);
    }
}

int ServerSocket::accept(ClientSocket &clientSocket) const {
    //std::cout << "listen_fd" << listen_fd << std::endl;
    // FIXME 之前这里出报 Invalid arg
    //int clientfd = ::accept(listen_fd, (struct sockaddr*)&clientSocket.mAddr, &clientSocket.mLen);
    int clientfd = ::accept(listen_fd, NULL, NULL);

    // FIXME 由于之前listen_fd是阻塞的逻辑正确，但是对于非阻塞listen_fd不正确
//    if (clientfd < 0) {
//        std::cout << "accept error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
//        exit(0);
//    }
    if (clientfd < 0) {
        if ((errno == EWOULDBLOCK) || (errno == EAGAIN))
            return clientfd;
        std::cout << "accept error in file <" << __FILE__ << "> "<< "at " << __LINE__ << std::endl;
        std::cout << "clientfd:" << clientfd << std::endl;
        perror("accpet error");
        //exit(0);
    }
    //std::cout << "accept a client： " << clientfd << std::endl;
    clientSocket.fd = clientfd;
    return clientfd;
}

void ServerSocket::close() {
    if (listen_fd >= 0) {
        ::close(listen_fd);
        //std::cout << "定时器超时关闭, 文件描述符:" << listen_fd << std::endl;
        listen_fd = -1;
    }
}
ServerSocket::~ServerSocket() {
    close();
}

void ClientSocket::close() {
    if (fd >= 0) {
        //std::cout << "文件描述符关闭: " << fd <<std::endl;
        ::close(fd);
        fd = -1;
    }
}
ClientSocket::~ClientSocket() {
    close();
}



