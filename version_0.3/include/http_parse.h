/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace csguide_webserver {

#define CR '\r'
#define LF '\n'
#define LINE_END '\0'
#define PASS

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
}  // namespace csguide_webserver
