//
// Created by marvinle on 2019/2/1 2:54 PM.
//

#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include "Socket.h"
#include "HttpParse.h"
#include "HttpResponse.h"
#include "HttpData.h"


#define BUFFERSIZE 1024

namespace server {


    class HttpServer {
    public:
        explicit HttpServer(int port = 80, const char *ip = nullptr): serverSocket(port, ip){
            serverSocket.bind();
            serverSocket.listen();
        }


        void run();
        void  do_request(std::shared_ptr<void> arg);

    private:

        void header(std::shared_ptr<http::HttpData>);
        void static_file(std::shared_ptr<http::HttpData>, const char*);
        void send(std::shared_ptr<http::HttpData>);
        void getMime(std::shared_ptr<http::HttpData>);

        nsocket::ServerSocket serverSocket;

    };
}

#endif //WEBSERVER_SERVER_H
