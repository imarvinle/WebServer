/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <memory>

#include "HttpData.h"
#include "HttpParse.h"
#include "HttpResponse.h"
#include "Socket.h"

namespace csguide_webserver {

#define BUFFERSIZE 2048

class HttpServer {
  public:
  enum FileState { FILE_OK, FIlE_NOT_FOUND, FILE_FORBIDDEN };

  public:
  HttpServer(const std::string& base_path, int port = 80,
             const char* ip = nullptr)
      : base_path_(base_path), serverSocket(port, ip) {
    serverSocket.bind();
    serverSocket.listen();
  }

  void run(int, int max_queue_size = 10000);

  void do_request(std::shared_ptr<void> arg);

  private:
  void header(std::shared_ptr<HttpData>);

  FileState static_file(std::shared_ptr<HttpData>,
                        const std::string& base_path);

  void send(std::shared_ptr<HttpData>, FileState);

  void getMime(std::shared_ptr<HttpData>);
  void hanleIndex();

  private:
  std::string base_path_;
  ServerSocket serverSocket;
};

}  // namespace csguide_webserver
