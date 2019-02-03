//
// Created by marvinle on 2019/2/1 2:54 PM.
//

#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include "ssocket.h"
#include "httpparse.h"
#include "httpresponse.h"


#define BUFFERSIZE 1024

namespace server {


    class HttpServer {
    public:
        explicit HttpServer(int port = 80, const char *ip = nullptr): serverSocket(port, ip){
            serverSocket.bind();
            serverSocket.listen();
        }


        void run();


    private:
        void do_request(const nsocket::ClientSocket&);
        void header(const http::HttpRequest&, http::HttpResponse&);
        void static_file(const http::HttpRequest&);
        void getMime(const http::HttpRequest&, http::HttpResponse&);

        nsocket::ServerSocket serverSocket;

    };
}

#endif //WEBSERVER_SERVER_H
