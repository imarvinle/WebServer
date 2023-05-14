/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "http_request.h"

namespace csguide_webserver {

struct MimeType {
  MimeType(const std::string &str) : type(str){};

  MimeType(const char *str) : type(str){};

  std::string type;
};

extern std::unordered_map<std::string, MimeType> Mime_map;

class HttpResponse {
public:
  enum HttpStatusCode { Unknow, k200Ok = 200, k403forbiden = 403, k404NotFound = 404 };

  explicit HttpResponse(bool mkeep = true)
      : status_code_(Unknow), keep_alive_(mkeep), mime_("text/html"), body_buufer(nullptr), version_(HttpRequest::HTTP_11) {}

  void SetStatusCode(HttpStatusCode code) { status_code_ = code; }

  void SetBody(const char *buf) { body_buufer = buf; }

  void SetContentLength(int len) { content_length_ = len; }

  void SetVersion(const HttpRequest::HTTP_VERSION &version) { version_ = version; }

  void SetStatusMsg(const std::string &msg) { status_msg_ = msg; }

  void SetFilePath(const std::string &path) { file_path_ = path; }

  void SetMime(const MimeType &mime) { mime_ = mime; }

  void SetKeepAlive(bool isalive) { keep_alive_ = isalive; }

  void AddHeader(const std::string &key, const std::string &value) { headers_[key] = value; }

  bool KeepAlive() const { return keep_alive_; }

  const HttpRequest::HTTP_VERSION Version() const { return version_; }

  const std::string &FilePath() const { return file_path_; }

  HttpStatusCode StatusCode() const { return status_code_; }

  const std::string &StatusMsg() const { return status_msg_; }

  void AppenBuffer(char *) const;

  ~HttpResponse() {
    if (body_buufer != nullptr) delete[] body_buufer;
  }

private:
  HttpStatusCode status_code_;
  HttpRequest::HTTP_VERSION version_;
  std::string status_msg_;
  bool keep_alive_;
  MimeType mime_;
  const char *body_buufer;
  int content_length_;
  std::string file_path_;
  std::unordered_map<std::string, std::string> headers_;
};

}  // namespace csguide_webserver
