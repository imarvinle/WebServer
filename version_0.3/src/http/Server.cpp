//
// Created by marvinle on 2019/2/1 7:34 PM.
//

#include "../../include/Server.h"
#include "../../include/HttpParse.h"
#include "../../include/HttpResponse.h"
#include "../../include/ThreadPool.h"
#include "../../include/HttpData.h"
#include "../../include/Epoll.h"
#include "../../include/Util.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <iostream>
#include <string>
#include <functional>
#include <sys/epoll.h>
#include <vector>
#include <cstring>



extern char basePath[300];


void HttpServer::run() {
    ThreadPool threadPool(4, 1000);

    //        ClientSocket *clientSocket = new ClientSocket;
//        serverSocket.accept(*clientSocket);
//        thread::ThreadTask *threadTask = new ThreadTask;
//        threadTask->process = std::bind(&HttpServer::do_request, this, std::placeholders::_1);
//        threadTask->arg = static_cast<void*>(clientSocket);
//        threadPool.append(threadTask);
    int epoll_fd = Epoll::init(1024);
    std::cout << "a|epoll_fd=" << epoll_fd << std::endl;
//        int ret = setnonblocking(epoll_fd);
//        if (ret < 0) {
//            std::cout << "epoll_fd set nonblocking error" << std::endl;
//        }
    std::shared_ptr<HttpData> httpData(new HttpData());
    httpData->epoll_fd = epoll_fd;
    serverSocket.epoll_fd = epoll_fd;   // 之前就是这里忘了添加,导致穿进去的serverSocket具有不正确的epoll_fd

    __uint32_t event = (EPOLLIN | EPOLLET);
    Epoll::addfd(epoll_fd, serverSocket.listen_fd, event, httpData);

    while (true) {


//        epoll_event eventss;
//        epoll_event events[1024];
//        eventss.data.fd = serverSocket.listen_fd;
//        eventss.events = EPOLLIN | EPOLLET;
//
//        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serverSocket.listen_fd, &eventss);
//        int ret = epoll_wait(epoll_fd, events, 1024, -1);
//        if (ret > 0) {
//            std::cout << "ret =" << ret << std::endl;
//        } else {
//            std::cout << "ret =" << ret << std::endl;
//        }

        //test end

        std::vector<std::shared_ptr<HttpData>> events = Epoll::poll(serverSocket, 1024, -1);

        for (auto& req : events) {
            threadPool.append(req, std::bind(&HttpServer::do_request, this, std::placeholders::_1));
        }
    }
}


void HttpServer::do_request(std::shared_ptr<void> arg) {
    std::shared_ptr<HttpData> sharedHttpData = std::static_pointer_cast<HttpData>(arg);

    char buffer[BUFFERSIZE];

    bzero(buffer, BUFFERSIZE);
    int check_index = 0, read_index = 0, start_line = 0;
    ssize_t recv_data;
    HttpRequestParser::PARSE_STATE  parse_state = HttpRequestParser::PARSE_REQUESTLINE;

    while (true) {

        // FIXME 这里也是同样的，由于是非阻塞IO，所以返回-1 也不一定是错误，还需判断error
        recv_data = recv(sharedHttpData->clientSocket_->fd, buffer + read_index, BUFFERSIZE - read_index, 0);
        if (recv_data == -1) {
            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                return;  // FIXME 请求不完整该怎么办，继续加定时器吗？还是直接关闭
            }
            std::cout << "reading faild" << std::endl;
            return;
        }
        // todo 返回值为 0对端关闭, 这边也应该关闭定时器

        if (recv_data == 0) {
            std::cout << "connection closed by peer" << std::endl;
            break;
        }
        read_index += recv_data;

        HttpRequestParser::HTTP_CODE  retcode = HttpRequestParser::parse_content(
                buffer, check_index, read_index, parse_state, start_line, *sharedHttpData->request_);

        if (retcode == HttpRequestParser::NO_REQUEST) {
            continue;
        }

        if (retcode == HttpRequestParser::GET_REQUEST) {
            // 检查 keep_alive选项
            auto it = sharedHttpData->request_->mHeaders.find(HttpRequest::Connection);
            if (it != sharedHttpData->request_->mHeaders.end()) {
                if (it->second == "keep_alive") {
                    sharedHttpData->response_->setKeepAlive(true);
                } else {
                    sharedHttpData->response_->setKeepAlive(false);
                }
            }
            header(sharedHttpData);
            getMime(sharedHttpData);
            static_file(sharedHttpData, "/Users/lichunlin/CLionProjects/webserver/version_0.1");
            send(sharedHttpData);

        } else {
            // todo Bad Request 应该关闭定时器
            std::cout << "Bad Request" << std::endl;
        }
    }
}

void HttpServer::header(std::shared_ptr<HttpData> httpData) {
    if (httpData->request_->mVersion == HttpRequest::HTTP_11) {
        httpData->response_->setVersion(HttpRequest::HTTP_11);
    } else {
        httpData->response_->setVersion(HttpRequest::HTTP_10);
    }
    httpData->response_->addHeader("Server", "LC WebServer");
}


// 获取Mime 同时设置path到response
void HttpServer::getMime(std::shared_ptr<HttpData> httpData) {
    std::string filepath = httpData->request_->mUri;
    std::string mime;
    int pos;
    if ((pos = filepath.rfind('?')) != std::string::npos) {
        filepath.erase(filepath.rfind('?'));
    }

    if (filepath.rfind('.') != std::string::npos){
        mime = filepath.substr(filepath.rfind('.'));
    }
    decltype(Mime_map)::iterator it;

    if ((it = Mime_map.find(mime)) != Mime_map.end()) {
        httpData->response_->setMime(it->second);
    } else {
        httpData->response_->setMime(Mime_map.find("default")->second);
    }
    httpData->response_->setFilePath(filepath);
}

void HttpServer::static_file(std::shared_ptr<HttpData> httpData, const char *basepath) {
    struct stat file_stat;
    char file[strlen(basepath) + strlen(httpData->response_->filePath().c_str())+1];
    strcpy(file, basepath);
    strcat(file, httpData->response_->filePath().c_str());

    if (stat(file, &file_stat) < 0) {
        httpData->response_->setStatusCode(HttpResponse::k404NotFound);
        httpData->response_->setStatusMsg("Not Found");
        httpData->response_->setFilePath(std::string(basepath)+"/404.html");
        return;
    }

    if(!S_ISREG(file_stat.st_mode)){
        httpData->response_->setStatusCode(HttpResponse::k403forbiden);
        httpData->response_->setStatusMsg("ForBidden");
        httpData->response_->setFilePath(std::string(basepath)+"/403.html");
        return;
    }

    httpData->response_->setStatusCode(HttpResponse::k200Ok);
    httpData->response_->setStatusMsg("OK");
    httpData->response_->setFilePath(file);
    return;
}

void HttpServer::send(std::shared_ptr<HttpData> httpData) {
    char header[BUFFERSIZE];
    bzero(header, '\0');
    const char *internal_error = "Internal Error";
    struct stat file_stat;
    httpData->response_->appenBuffer(header);
    if (stat(httpData->response_->filePath().c_str(), &file_stat) < 0) {
        sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(internal_error));
        sprintf(header, "%s%s", header, internal_error);
        ::send(httpData->clientSocket_->fd, header, strlen(header), 0);
        return;
    }

    int filefd = ::open(httpData->response_->filePath().c_str(), O_RDONLY);
    if (filefd < 0) {
        sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(internal_error));
        sprintf(header, "%s%s", header, internal_error);
        ::send(httpData->clientSocket_->fd, header, strlen(header), 0);
        return;
    }

    sprintf(header,"%sContent-length: %d\r\n\r\n", header, file_stat.st_size);
    ::send(httpData->clientSocket_->fd, header, strlen(header), 0);
    void *mapbuf = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, filefd, 0);
    ::send(httpData->clientSocket_->fd, mapbuf, file_stat.st_size, 0);
    munmap(mapbuf, file_stat.st_size);
    close(filefd);
    return;
err:
    sprintf(header, "%sContent-length: %d\r\n\r\n", header, strlen(internal_error));
    sprintf(header, "%s%s", header, internal_error);
    ::send(httpData->clientSocket_->fd, header, strlen(header), 0);
    return;
}