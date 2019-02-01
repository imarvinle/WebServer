//
// Created by marvinle on 2019/2/1 2:54 PM.
//

#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include "ssocket.h"

#define BUFFERSIZE 1024

namespace server {

    class HttpServer {
    public:
        HttpServer(int port = 8080, const char *ip = nullptr);

        void run();

    private:
        nsocket::ServerSocket serverSocket;

    };
}

#endif //WEBSERVER_SERVER_H
