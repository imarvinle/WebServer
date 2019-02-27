//
// Created by marvinle on 2019/2/1 2:54 PM.
//

//#ifndef WEBSERVER_SERVER_H
//#define WEBSERVER_SERVER_H
#pragma once

#include "Socket.h"
#include "HttpParse.h"
#include "HttpResponse.h"
#include "HttpData.h"
#include <memory>



#define BUFFERSIZE 1024


class HttpServer {
public:
    explicit HttpServer(int port = 80, const char *ip = nullptr) : serverSocket(port, ip) {
        serverSocket.bind();
        serverSocket.listen();
    }


    void run();

    void do_request(std::shared_ptr<void> arg);

private:

    void header(std::shared_ptr<HttpData>);

    void static_file(std::shared_ptr<HttpData>, const char *);

    void send(std::shared_ptr<HttpData>);

    void getMime(std::shared_ptr<HttpData>);

    ServerSocket serverSocket;

};


//#endif //WEBSERVER_SERVER_H
