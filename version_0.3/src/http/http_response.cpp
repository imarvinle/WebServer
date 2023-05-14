/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include <string>

#include "../../include/http_response.h"

namespace csguide_webserver {

std::unordered_map<std::string, MimeType> Mime_map = {
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".xhtml", "application/xhtml+xml"},
    {".txt", "text/plain"},
    {".rtf", "application/rtf"},
    {".pdf", "application/pdf"},
    {".word", "application/msword"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".au", "audio/basic"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".tar", "application/x-tar"},
    {".svg", "image/svg+xml"},
    {".css", "text/css"},
    {"", "text/plain"},
    {"default", "text/plain"}};

void HttpResponse::appenBuffer(char *buffer) const {
  // 版本
  if (mVersion == HttpRequest::HTTP_11) {
    sprintf(buffer, "HTTP/1.1 %d %s\r\n", mStatusCode, mStatusMsg.c_str());
  } else {
    sprintf(buffer, "HTTP/1.0 %d %s\r\n", mStatusCode, mStatusMsg.c_str());
  }
  // 头部字段
  for (auto it = mHeaders.begin(); it != mHeaders.end(); it++) {
    sprintf(buffer, "%s%s: %s\r\n", buffer, it->first.c_str(),
            it->second.c_str());
  }
  sprintf(buffer, "%sContent-type: %s\r\n", buffer, mMime.type.c_str());
  // keep_alive
  if (keep_alive_) {
    sprintf(buffer, "%sConnection: keep-alive\r\n", buffer);
  } else {
    sprintf(buffer, "%sConnection: close\r\n", buffer);
  }
}

}  // namespace csguide_webserver
