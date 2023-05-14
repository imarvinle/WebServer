//
// Created by marvinle on 2019/2/1 7:34 PM.
//

#include "../include/server.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <string>

#include "../include/HttpResponse.h"
#include "../include/ThreadPool.h"
#include "../include/httpparse.h"

using namespace server;
using namespace nsocket;
using namespace http;

extern char basePath[300];

void HttpServer::run() {
  thread::ThreadPool threadPool(4, 1000);
  while (true) {
    ClientSocket *clientSocket = new ClientSocket;
    serverSocket.accept(*clientSocket);
    thread::ThreadTask *threadTask = new thread::ThreadTask;
    threadTask->process =
        std::bind(&HttpServer::do_request, this, std::placeholders::_1);
    threadTask->arg = static_cast<void *>(clientSocket);
    threadPool.append(threadTask);
  }
}

void HttpServer::do_request(void *arg) {
  ClientSocket clientSocket = *static_cast<ClientSocket *>(arg);

  char buffer[BUFFERSIZE];

  bzero(buffer, BUFFERSIZE);
  int check_index = 0, read_index = 0, start_line = 0;
  ssize_t recv_data;
  http::HttpRequestParser::PARSE_STATE parse_state =
      http::HttpRequestParser::PARSE_REQUESTLINE;

  while (true) {
    http::HttpRequest request;

    recv_data =
        recv(clientSocket.fd, buffer + read_index, BUFFERSIZE - read_index, 0);
    if (recv_data == -1) {
      std::cout << "reading faild" << std::endl;
      return;
    }
    if (recv_data == 0) {
      std::cout << "connection closed by peer" << std::endl;
      break;
    }
    read_index += recv_data;

    http::HttpRequestParser::HTTP_CODE retcode =
        http::HttpRequestParser::parse_content(
            buffer, check_index, read_index, parse_state, start_line, request);

    if (retcode == http::HttpRequestParser::NO_REQUEST) {
      continue;
    }
    std::cout << request << std::endl;

    if (retcode == http::HttpRequestParser::GET_REQUEST) {
      HttpResponse response(true);
      header(request, response);
      getMime(request, response);
      static_file(response, basePath);
      send(response, clientSocket);
    } else {
      std::cout << "Bad Request" << std::endl;
    }
  }
}

void HttpServer::header(const HttpRequest &request, HttpResponse &response) {
  if (request.mVersion == HttpRequest::HTTP_11) {
    response.setVersion(HttpRequest::HTTP_11);
  } else {
    response.setVersion(HttpRequest::HTTP_10);
  }
  response.addHeader("Server", "LC WebServer");
}

// 获取Mime 同时设置path到response
void HttpServer::getMime(const http::HttpRequest &request,
                         http::HttpResponse &response) {
  std::string filepath = request.mUri;
  std::string mime;
  int pos;
  if ((pos = filepath.rfind('?')) != std::string::npos) {
    filepath.erase(filepath.rfind('?'));
  }

  if (filepath.rfind('.') != std::string::npos) {
    mime = filepath.substr(filepath.rfind('.'));
  }
  decltype(http::Mime_map)::iterator it;

  if ((it = http::Mime_map.find(mime)) != http::Mime_map.end()) {
    response.setMime(it->second);
  } else {
    response.setMime(http::Mime_map.find("default")->second);
  }
  response.setFilePath(filepath);
}

void HttpServer::static_file(HttpResponse &response, const char *basepath) {
  struct stat file_stat;
  char file[strlen(basepath) + strlen(response.filePath().c_str()) + 1];
  strcpy(file, basepath);
  strcat(file, response.filePath().c_str());

  if (stat(file, &file_stat) < 0) {
    response.setStatusCode(HttpResponse::k404NotFound);
    response.setStatusMsg("Not Found");
    response.setFilePath(std::string(basepath) + "/404.html");
    return;
  }

  if (!S_ISREG(file_stat.st_mode)) {
    response.setStatusCode(HttpResponse::k403forbiden);
    response.setStatusMsg("ForBidden");
    response.setFilePath(std::string(basepath) + "/403.html");
    return;
  }

  response.setStatusCode(HttpResponse::k200Ok);
  response.setStatusMsg("OK");
  response.setFilePath(file);
  return;
}

void HttpServer::send(const http::HttpResponse &response,
                      const nsocket::ClientSocket &clientSocket) {
  char header[BUFFERSIZE];
  bzero(header, '\0');
  const char *internal_error = "Internal Error";
  struct stat file_stat;
  response.appenBuffer(header);
  if (stat(response.filePath().c_str(), &file_stat) < 0) {
    sprintf(header, "%sContent-length: %d\r\n\r\n", header,
            strlen(internal_error));
    sprintf(header, "%s%s", header, internal_error);
    ::send(clientSocket.fd, header, strlen(header), 0);
    return;
  }

  int filefd = ::open(response.filePath().c_str(), O_RDONLY);
  if (filefd < 0) {
    sprintf(header, "%sContent-length: %d\r\n\r\n", header,
            strlen(internal_error));
    sprintf(header, "%s%s", header, internal_error);
    ::send(clientSocket.fd, header, strlen(header), 0);
    return;
  }

  sprintf(header, "%sContent-length: %d\r\n\r\n", header, file_stat.st_size);
  ::send(clientSocket.fd, header, strlen(header), 0);
  void *mapbuf =
      mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, filefd, 0);
  ::send(clientSocket.fd, mapbuf, file_stat.st_size, 0);
  munmap(mapbuf, file_stat.st_size);
  close(filefd);
  return;
err:
  sprintf(header, "%sContent-length: %d\r\n\r\n", header,
          strlen(internal_error));
  sprintf(header, "%s%s", header, internal_error);
  ::send(clientSocket.fd, header, strlen(header), 0);
  return;
}