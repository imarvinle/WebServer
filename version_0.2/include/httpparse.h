//
// Created by marvinle on 2019/2/1 10:57 AM.
//

#ifndef WEBSERVER_HTTPPARSE_H
#define WEBSERVER_HTTPPARSE_H

#include <iostream>
#include <string>
#include <unordered_map>

#define CR '\r'
#define LF '\n'
#define LINE_END '\0'
#define PASS

namespace http {

class HttpRequest;

std::ostream &operator<<(std::ostream &, const HttpRequest &);

class HttpRequestParser {
  public:
  enum LINE_STATE { LINE_OK = 0, LINE_BAD, LINE_MORE };
  enum PARSE_STATE { PARSE_REQUESTLINE = 0, PARSE_HEADER, PARSE_BODY };
  enum HTTP_CODE {
    NO_REQUEST,
    GET_REQUEST,
    BAD_REQUEST,
    FORBIDDEN_REQUEST,
    INTERNAL_ERROR,
    CLOSED_CONNECTION
  };

  static LINE_STATE parse_line(char *buffer, int &checked_index,
                               int &read_index);
  static HTTP_CODE parse_requestline(char *line, PARSE_STATE &parse_state,
                                     HttpRequest &request);
  static HTTP_CODE parse_headers(char *line, PARSE_STATE &parse_state,
                                 HttpRequest &request);
  static HTTP_CODE parse_body(char *body, HttpRequest &request);
  static HTTP_CODE parse_content(char *buffer, int &check_index,
                                 int &read_index, PARSE_STATE &parse_state,
                                 int &start_line, HttpRequest &request);
};

struct HttpRequest {
  friend std::ostream &operator<<(std::ostream &, const HttpRequest &);

  enum HTTP_VERSION { HTTP_10 = 0, HTTP_11, VERSION_NOT_SUPPORT };
  enum HTTP_METHOD { GET = 0, POST, PUT, DELETE, METHOD_NOT_SUPPORT };
  enum HTTP_HEADER {
    Host = 0,
    User_Agent,
    Connection,
    Accept_Encoding,
    Accept_Language,
    Accept,
    Cache_Control,
    Upgrade_Insecure_Requests
  };
  struct EnumClassHash {
    template <typename T>
    std::size_t operator()(T t) const {
      return static_cast<std::size_t>(t);
    }
  };

  static std::unordered_map<std::string, HTTP_HEADER> header_map;

  HttpRequest(std::string url = std::string(""),
              HTTP_METHOD method = METHOD_NOT_SUPPORT,
              HTTP_VERSION version = VERSION_NOT_SUPPORT)
      : mMethod(method),
        mVersion(version),
        mUri(url),
        mContent(nullptr),
        mHeaders(
            std::unordered_map<HTTP_HEADER, std::string, EnumClassHash>()){};

  HTTP_METHOD mMethod;
  HTTP_VERSION mVersion;
  std::string mUri;
  char *mContent;
  std::unordered_map<HTTP_HEADER, std::string, EnumClassHash> mHeaders;
};

}  // namespace http

#endif  // WEBSERVER_HTTPPARSE_H
