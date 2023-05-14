//
// Created by marvinle on 2019/2/2 1:37 PM.
//

#include "../include/httpresponse.h"

#include <string>

std::unordered_map<std::string, http::MimeType> http::Mime_map = {
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
    {".css", "text/css"},
    {"", "text/plain"},
    {"default", "text/plain"}};

void http::HttpResponse::appenBuffer(char *buffer) const {
  if (mVersion == HttpRequest::HTTP_11) {
    sprintf(buffer, "HTTP/1.1 %d %s\r\n", mStatusCode, mStatusMsg.c_str());
  } else {
    sprintf(buffer, "HTTP/1.0 %d %s\r\n", mStatusCode, mStatusMsg.c_str());
  }

  for (auto it = mHeaders.begin(); it != mHeaders.end(); it++) {
    sprintf(buffer, "%s%s: %s\r\n", buffer, it->first.c_str(),
            it->second.c_str());
  }
  sprintf(buffer, "%sContent-type: %s\r\n", buffer, mMime.type.c_str());
  if (mCloseConnection) {
    sprintf(buffer, "%sConnection: close\r\n", buffer);
  } else {
    sprintf(buffer, "%sConnection: keep-alive\r\n", buffer);
  }
}
