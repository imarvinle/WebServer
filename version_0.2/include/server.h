//
// Created by marvinle on 2019/2/1 2:54 PM.
//

#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include "HttpResponse.h"
#include "httpparse.h"
#include "ssocket.h"

#define BUFFERSIZE 1024

namespace server {

class HttpServer {
  public:
  explicit HttpServer(int port = 80, const char *ip = nullptr)
      : serverSocket(port, ip) {
    serverSocket.bind();
    serverSocket.listen();
  }

  void run();
  void do_request(void *args);

  private:
  void header(const http::HttpRequest &, http::HttpResponse &);
  void static_file(http::HttpResponse &, const char *);
  void send(const http::HttpResponse &, const nsocket::ClientSocket &);
  void getMime(const http::HttpRequest &, http::HttpResponse &);

  nsocket::ServerSocket serverSocket;
};
}  // namespace server

#endif  // WEBSERVER_SERVER_H
