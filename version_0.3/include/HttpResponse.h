//
// Created by marvinle on 2019/2/2 1:02 PM.
//

//#ifndef WEBSERVER_HTTPRESPONSE_H
//#define WEBSERVER_HTTPRESPONSE_H
#pragma once

#include "HttpRequest.h"

#include <string>
#include <unordered_map>
#include <memory>



struct MimeType {
    MimeType(const std::string &str) : type(str) {};

    MimeType(const char *str) : type(str) {};

    std::string type;
};

extern std::unordered_map<std::string, MimeType> Mime_map;

class HttpResponse {

public:
    enum HttpStatusCode {
        Unknow,
        k200Ok = 200,
        k403forbiden = 403,
        k404NotFound = 404
    };

    explicit HttpResponse(bool mkeep = true)
            : mStatusCode(Unknow), keep_alive_(mkeep), mMime("text/html"), mBody(nullptr),
              mVersion(HttpRequest::HTTP_11) {}

    void setStatusCode(HttpStatusCode code) {
        mStatusCode = code;
    }

    void setBody(const char *buf) {
        mBody = buf;
    }

    void setContentLength(int len) {
        mContentLength = len;
    }

    void setVersion(const HttpRequest::HTTP_VERSION &version) {
        mVersion = version;
    }

    void setStatusMsg(const std::string &msg) {
        mStatusMsg = msg;
    }

    void setFilePath(const std::string &path) {
        mFilePath = path;
    }

    void setMime(const MimeType &mime) {
        mMime = mime;
    }

    void setKeepAlive(bool isalive) {
        keep_alive_ = isalive;
    }

    void addHeader(const std::string &key, const std::string &value) {
        mHeaders[key] = value;
    }

    bool keep_alive() const {
        return keep_alive_;
    }

    const HttpRequest::HTTP_VERSION version() const {
        return mVersion;
    }

    const std::string &filePath() const {
        return mFilePath;
    }

    HttpStatusCode statusCode() const {
        return mStatusCode;
    }

    const std::string &statusMsg() const {
        return mStatusMsg;
    }

    void appenBuffer(char *) const;

    ~HttpResponse() {
        if (mBody != nullptr)
            delete[] mBody;
    }

private:
    HttpStatusCode mStatusCode;
    HttpRequest::HTTP_VERSION mVersion;
    std::string mStatusMsg;
    bool keep_alive_;
    MimeType mMime;
    const char *mBody;
    int mContentLength;
    std::string mFilePath;
    std::unordered_map<std::string, std::string> mHeaders;
};


//#endif //WEBSERVER_HTTPRESPONSE_H
