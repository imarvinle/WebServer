//
// Created by marvinle on 2019/2/2 1:02 PM.
//

#ifndef WEBSERVER_HTTPRESPONSE_H
#define WEBSERVER_HTTPRESPONSE_H

#include <string>
#include <unordered_map>

namespace http {

    struct MimeType {
        MimeType(const std::string& str): type(str) {};
        MimeType(const char *str): type(str) {};

        std:: string type;
    };

    extern std::unordered_map<std::string, http::MimeType> Mime_map;

    class HttpResponse {

    public:
        enum HttpStatusCode {
            Unknow,
            k200Ok = 200,
            k404NotFound = 404
        };

        explicit HttpResponse(bool close)
                : mStatusCode(Unknow), mCloseConnection(close), mMime("text/html"),mBody(nullptr){}

        void setStatusCode(HttpStatusCode code) {
            mStatusCode = code;
        }
        void setBody(const char *buf) {
            mBody = buf;
        }
        void setStatusMsg(const std::string &msg) {
            mStatusMsg = msg;
        }
        void addHeader(const std::string &key, const std::string &value){
            mHeaders[key] = value;
        }

        bool closeConnection() const  {
            return mCloseConnection;
        }

        void apendBuffer(char *buf) const;

        ~HttpResponse() {
            if (mBody != nullptr)
                delete[] mBody;
        }


    private:
        HttpStatusCode mStatusCode;
        std::string mStatusMsg;
        bool mCloseConnection;
        MimeType mMime;
        const char *mBody;
        std::unordered_map<std::string, std::string> mHeaders;
    };


}

#endif //WEBSERVER_HTTPRESPONSE_H
