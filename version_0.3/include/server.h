/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <memory>

#include "http_data.h"
#include "http_parse.h"
#include "http_response.h"
#include "socket.h"

namespace csguide_webserver {

#define BUFFERSIZE 2048

class HttpServer {
public:
  enum FileState { FILE_OK, FIlE_NOT_FOUND, FILE_FORBIDDEN };

public:
  HttpServer(const std::string& base_path, int port = 80, const char* ip = nullptr)
      : base_path_(base_path), serverSocket(port, ip) {
      serverSocket.Bind();
      serverSocket.Listen();
  }

  void Run(int, int max_queue_size = 10000);

  void DoRequest(std::shared_ptr<void> arg);

private:
  void Header(std::shared_ptr<HttpData>);

  FileState StaticFile(std::shared_ptr<HttpData>, const std::string& base_path);

  void Send(std::shared_ptr<HttpData>, FileState);

  void GetMime(std::shared_ptr<HttpData>);

private:
  std::string base_path_;
  ServerSocket serverSocket;
};

}  // namespace csguide_webserver
