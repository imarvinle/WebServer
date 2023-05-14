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

struct ServerConf {
  int thread_num = 4;
  int port = 8080;
  bool daemon = true;
  std::string root = "./pages/";  // 这是默认目录，也就是当前项目的 pages 子目录
};

class HttpServer {
public:
  enum FileState { FILE_OK, FIlE_NOT_FOUND, FILE_FORBIDDEN };

public:
  HttpServer(const ServerConf& server_conf, const char* ip = nullptr)
      : server_conf_(server_conf), serverSocket(server_conf.port, ip) {
    serverSocket.Bind();
    serverSocket.Listen();
  }

  void Run(int max_queue_size = 10000);

  void DoRequest(std::shared_ptr<void> arg);

private:
  void Header(std::shared_ptr<HttpData> http_data);

  FileState StaticFile(std::shared_ptr<HttpData> http_data);

  void Send(std::shared_ptr<HttpData> http_data, FileState file_state);

  void GetMime(std::shared_ptr<HttpData> http_data);

private:
  const ServerConf& server_conf_;
  ServerSocket serverSocket;
};

}  // namespace csguide_webserver
