/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include "httpparse.h"
#include "httpresponse.h"
#include "ssocket.h"

#define BUFFERSIZE 1024

namespace server {

class HttpServer {
public:
  explicit HttpServer(int port = 80, const char *ip = nullptr) : serverSocket(port, ip) {
    serverSocket.bind();
    serverSocket.listen();
  }

  void run();

private:
  void do_request(const nsocket::ClientSocket &);
  void header(const http::HttpRequest &, http::HttpResponse &);
  void static_file(http::HttpResponse &, const char *);
  void send(const http::HttpResponse &, const nsocket::ClientSocket &);
  void getMime(const http::HttpRequest &, http::HttpResponse &);

  nsocket::ServerSocket serverSocket;
};
}  // namespace server

#endif  // WEBSERVER_SERVER_H
