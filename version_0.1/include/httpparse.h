//
// Created by marvinle on 2019/2/1 10:57 AM.
//

#ifndef WEBSERVER_HTTPPARSE_H
#define WEBSERVER_HTTPPARSE_H

#include <string>
#include <unordered_map>

#define CR '\r'
#define LF '\n'
#define LINE_END '\0'
#define PASS

namespace parse {

    class HttpRequest;

    class HttpRequestParser {

        enum LINE_STATE { LINE_OK = 0, LINE_BAD, LINE_MORE };
        enum PARSE_STATE { PARSE_REQUESTLINE = 0, PARSE_HEADER, PARSE_BODY };
        enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR,
            CLOSED_CONNECTION };

    public:
        LINE_STATE parse_line(char *buffer, int &checked_index, int &read_index);
        HTTP_CODE parse_requestline(char *line, PARSE_STATE &parse_state, HttpRequest &request);
        HTTP_CODE parse_headers(char *line, PARSE_STATE &parse_state, HttpRequest &request);

    private:


    };

    struct HttpRequest {
        enum HTTP_VERSION { HTTP_10 = 0, HTTP_11, VERSION_NOT_SUPPORT};
        enum HTTP_METHOD { GET = 0, POST, PUT, DELETE, METHOD_NOT_SUPPORT};
        enum HTTP_HEADER { Host = 0, User_Agent, Connection, Accept_Encoding, Accept_Language};
        struct EnumClassHash
        {
            template <typename T>
            std::size_t operator()(T t) const
            {
                return static_cast<std::size_t>(t);
            }
        };

        static std::unordered_map<std::string, HTTP_HEADER> header_map;

        HttpRequest(std::string url = std::string(""), HTTP_METHOD method = METHOD_NOT_SUPPORT, HTTP_VERSION version = VERSION_NOT_SUPPORT):
                mMethod(method), mVersion(version), mUrl(url), mContent(nullptr),
                mHeaders(std::unordered_map<HTTP_HEADER, std::string, EnumClassHash>()){};

        HTTP_METHOD mMethod;
        HTTP_VERSION mVersion;
        std::string mUrl;
        char *mContent;
        std::unordered_map<HTTP_HEADER, std::string, EnumClassHash> mHeaders;
    };
}

#endif //WEBSERVER_HTTPPARSE_H
