//
// Created by marvinle on 2019/2/1 10:59 AM.
//

#include "../include/httpparse.h"
#include <cstring>
#include <iostream>


std::unordered_map<std::string, parse::HttpRequest::HTTP_HEADER> parse::HttpRequest::header_map = {
        {"HOST", parse::HttpRequest::Host},
        {"USER-AGENT", parse::HttpRequest::User_Agent},
        {"CONNECTION", parse::HttpRequest::Connection},
        {"ACCEPT_ENCODING", parse::HttpRequest::Accept_Encoding},
        {"ACCEPT_LANGUAGE", parse::HttpRequest::Accept_Language}
};

// 解析一行内容, buffer[checked_index, read_index)
// check_index是需要分析的第一个字符， read_index已经读取数据末尾下一个字符
parse::HttpRequestParser::LINE_STATE parse::HttpRequestParser::parse_line(char *buffer, int &checked_index, int &read_index){
    char temp;
    for (; checked_index < read_index; checked_index++) {
        temp = buffer[checked_index];
        if (temp == CR) {
            // 到末尾，需要读入
            if (checked_index+1 == read_index)
                return LINE_MORE;
            // 完整 "\r\n"
            if (buffer[checked_index+1] == LF) {
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
parse::HttpRequestParser::HTTP_CODE parse::HttpRequestParser::parse_requestline(char *line, PARSE_STATE &parse_state, HttpRequest &request){
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
    if (!version){
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn(version, " \t");

    // HTTP/1.1 后面可能还存在空白字符
    if (strncasecmp("HTTP/1.1", version, 8) == 0){
        request.mVersion = HttpRequest::HTTP_11;
    } else if (strncasecmp("HTTP/1.0", version, 8) == 0){
        request.mVersion = HttpRequest::HTTP_10;
    } else {
        return BAD_REQUEST;
    }

    if (strncasecmp(url, "http://", 7) == 0) {
        url += 7;
        url = strchr(url, '/');
    } else if (strncasecmp(url, "/" , 1) == 0){
        PASS;
    } else {
        return BAD_REQUEST;
    }

    if(!url || *url != '/'){
        return BAD_REQUEST;
    }
    request.mUrl = std::string(url);
    // 分析头部字段
    parse_state = PARSE_HEADER;
    return NO_REQUEST;
}

// 分析头部字段
parse::HttpRequestParser::HTTP_CODE parse::HttpRequestParser::parse_headers(char *line, PARSE_STATE &parse_state, HttpRequest &request){

    if (*line == '\0') {
        parse_state = PARSE_BODY;
        return GET_REQUEST;
    }

    char *key[20], value[100];
    sscanf(line, "%[^:]:%[^:]", key, value);
    return BAD_REQUEST;
}




