//
// Created by marvinle on 2019/2/1 10:59 AM.
//

#include "../../include/HttpRequest.h"
#include "../../include/HttpParse.h"
#include "../../include/Util.h"

#include <string.h>
#include <iostream>
#include <algorithm>


std::unordered_map<std::string, HttpRequest::HTTP_HEADER> HttpRequest::header_map = {
        {"HOST",                      HttpRequest::Host},
        {"USER-AGENT",                HttpRequest::User_Agent},
        {"CONNECTION",                HttpRequest::Connection},
        {"ACCEPT-ENCODING",           HttpRequest::Accept_Encoding},
        {"ACCEPT-LANGUAGE",           HttpRequest::Accept_Language},
        {"ACCEPT",                    HttpRequest::Accept},
        {"CACHE-CONTROL",             HttpRequest::Cache_Control},
        {"UPGRADE-INSECURE-REQUESTS", HttpRequest::Upgrade_Insecure_Requests}
};


// 解析一行内容, buffer[checked_index, read_index)
// check_index是需要分析的第一个字符， read_index已经读取数据末尾下一个字符
HttpRequestParser::LINE_STATE
HttpRequestParser::parse_line(char *buffer, int &checked_index, int &read_index) {
    char temp;
    for (; checked_index < read_index; checked_index++) {
        temp = buffer[checked_index];
        if (temp == CR) {
            // 到末尾，需要读入
            if (checked_index + 1 == read_index)
                return LINE_MORE;
            // 完整 "\r\n"
            if (buffer[checked_index + 1] == LF) {
                buffer[checked_index++] = LINE_END;
                buffer[checked_index++] = LINE_END;
                return LINE_OK;
            }

            return LINE_BAD;
        }
    }
    // 需要读入更多
    return LINE_MORE;
}

// 解析请求行
HttpRequestParser::HTTP_CODE
HttpRequestParser::parse_requestline(char *line, PARSE_STATE &parse_state, HttpRequest &request) {
    char *url = strpbrk(line, " \t");
    if (!url) {
        return BAD_REQUEST;
    }

    // 分割 method 和 url
    *url++ = '\0';

    char *method = line;

    if (strcasecmp(method, "GET") == 0) {
        request.mMethod = HttpRequest::GET;
    } else if (strcasecmp(method, "POST") == 0) {
        request.mMethod = HttpRequest::POST;
    } else if (strcasecmp(method, "PUT") == 0) {
        request.mMethod = HttpRequest::PUT;
    } else {
        return BAD_REQUEST;
    }

    url += strspn(url, " \t");
    char *version = strpbrk(url, " \t");
    if (!version) {
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn(version, " \t");

    // HTTP/1.1 后面可能还存在空白字符
    if (strncasecmp("HTTP/1.1", version, 8) == 0) {
        request.mVersion = HttpRequest::HTTP_11;
    } else if (strncasecmp("HTTP/1.0", version, 8) == 0) {
        request.mVersion = HttpRequest::HTTP_10;
    } else {
        return BAD_REQUEST;
    }

    if (strncasecmp(url, "http://", 7) == 0) {
        url += 7;
        url = strchr(url, '/');
    } else if (strncasecmp(url, "/", 1) == 0) {
        PASS;
    } else {
        return BAD_REQUEST;
    }

    if (!url || *url != '/') {
        return BAD_REQUEST;
    }
    request.mUri = std::string(url);
    // 分析头部字段
    parse_state = PARSE_HEADER;
    return NO_REQUEST;
}

// 分析头部字段
HttpRequestParser::HTTP_CODE
HttpRequestParser::parse_headers(char *line, PARSE_STATE &parse_state, HttpRequest &request) {

    if (*line == '\0') {
        if (request.mMethod == HttpRequest::GET) {
            return GET_REQUEST;
        }
        parse_state = PARSE_BODY;
        return NO_REQUEST;
    }

    // FIXME  char key[20]曾被缓冲区溢出, value[100]也被 chrome的user-agent 溢出
    char key[100], value[300];

    // FIXME 需要修改有些value里也包含了':'符号
    sscanf(line, "%[^:]:%[^:]", key, value);


    decltype(HttpRequest::header_map)::iterator it;
    std::string key_s(key);
    std::transform(key_s.begin(), key_s.end(), key_s.begin(), ::toupper);
    std::string value_s(value);
//    if (key_s == std::string("UPGRADE-INSECURE-REQUESTS")) {
//        return NO_REQUEST;
//    }

    if ((it = HttpRequest::header_map.find(trim(key_s))) != (HttpRequest::header_map.end())) {
        request.mHeaders.insert(std::make_pair(it->second, trim(value_s)));
    } else {
        //std::cout << "Header no support: " << key << " : " << value << std::endl;
    }

    return NO_REQUEST;
}

// 解析body
HttpRequestParser::HTTP_CODE
HttpRequestParser::parse_body(char *body, HttpRequest &request) {
    request.mContent = body;
    return GET_REQUEST;
}

// http 请求入口
HttpRequestParser::HTTP_CODE
HttpRequestParser::parse_content(char *buffer, int &check_index, int &read_index,
                                 HttpRequestParser::PARSE_STATE &parse_state, int &start_line,
                                 HttpRequest &request) {

    LINE_STATE line_state = LINE_OK;
    HTTP_CODE retcode = NO_REQUEST;
    while ((line_state = parse_line(buffer, check_index, read_index)) == LINE_OK) {
        char *temp = buffer + start_line;      // 这一行在buffer中的起始位置
        start_line = check_index;              // 下一行起始位置

        switch (parse_state) {
            case PARSE_REQUESTLINE: {
                retcode = parse_requestline(temp, parse_state, request);
                if (retcode == BAD_REQUEST)
                    return BAD_REQUEST;

                break;
            }

            case PARSE_HEADER: {
                retcode = parse_headers(temp, parse_state, request);
                if (retcode == BAD_REQUEST) {
                    return BAD_REQUEST;
                } else if (retcode == GET_REQUEST) {
                    return GET_REQUEST;
                }
                break;
            }

            case PARSE_BODY: {
                retcode = parse_body(temp, request);
                if (retcode == GET_REQUEST) {
                    return GET_REQUEST;
                }
                return BAD_REQUEST;
            }
            default:
                return INTERNAL_ERROR;
        }
    }
    if (line_state == LINE_MORE) {
        return NO_REQUEST;
    } else {
        return BAD_REQUEST;
    }
}






