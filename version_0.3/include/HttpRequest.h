//
// Created by marvinle on 2019/2/26 6:36 PM.
//

//#ifndef WEBSERVER_HTTPREQUEST_H
//#define WEBSERVER_HTTPREQUEST_H
#pragma once


#include <iostream>
#include <unordered_map>


class HttpRequest;

std::ostream &operator<<(std::ostream &, const HttpRequest &);


struct HttpRequest {

    friend std::ostream &operator<<(std::ostream &, const HttpRequest &);

    enum HTTP_VERSION {
        HTTP_10 = 0, HTTP_11, VERSION_NOT_SUPPORT
    };
    enum HTTP_METHOD {
        GET = 0, POST, PUT, DELETE, METHOD_NOT_SUPPORT
    };
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
        template<typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    static std::unordered_map<std::string, HTTP_HEADER> header_map;

    HttpRequest(std::string url = std::string(""), HTTP_METHOD method = METHOD_NOT_SUPPORT,
                HTTP_VERSION version = VERSION_NOT_SUPPORT) :
            mMethod(method), mVersion(version), mUri(url), mContent(nullptr),
            mHeaders(std::unordered_map<HTTP_HEADER, std::string, EnumClassHash>()) {};

    HTTP_METHOD mMethod;
    HTTP_VERSION mVersion;
    std::string mUri;
    char *mContent;
    std::unordered_map<HTTP_HEADER, std::string, EnumClassHash> mHeaders;
};



//#endif //WEBSERVER_HTTPREQUEST_H
