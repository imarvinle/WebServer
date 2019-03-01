//
// Created by marvinle on 2019/2/1 2:54 PM.
//

//#ifndef WEBSERVER_SERVER_H
//#define WEBSERVER_SERVER_H
#pragma once

#include "Socket.h"
#include "HttpParse.h"
#include "HttpResponse.h"
#include "HttpData.h"
#include <memory>



#define BUFFERSIZE 2048

char NOT_FOUND_PAGE[] = "\"<html>\\n\"\n"
                        "\"<head><title>404 Not Found</title></head>\\n\"\n"
                        "\"<body bgcolor=\\\"white\\\">\\n\"\n"
                        "\"<center><h1>404 Not Found</h1></center>\\n\"\n"
                        "\"<hr><center>LC Server/0.3 (Linux)</center>\\n\"\n"
                        "\"</body>\\n\"\n"
                        "\"</html>\"";

char FORBIDDEN_PAGE[] = "<html>\n"
                        "<head><title>403 Forbidden</title></head>\n"
                        "<body bgcolor=\"white\">\n"
                        "<center><h1>403 Forbidden</h1></center>\n"
                        "<hr><center>LC WebServer/0.3 (Linux)</center>\n"
                        "</body>\n"
                        "</html>";

char INDEX_PAGE[] = "<!DOCTYPE html>\n"
                    "<html>\n"
                    "<head>\n"
                    "    <title>Welcome to LC!</title>\n"
                    "    <style>\n"
                    "        body {\n"
                    "            width: 35em;\n"
                    "            margin: 0 auto;\n"
                    "            font-family: Tahoma, Verdana, Arial, sans-serif;\n"
                    "        }\n"
                    "    </style>\n"
                    "</head>\n"
                    "<body>\n"
                    "<h1>Welcome to LC !</h1>\n"
                    "<p>If you see this page, the lc webserver is successfully installed and\n"
                    "    working. </p>\n"
                    "\n"
                    "<p>For online documentation and support please refer to\n"
                    "    <a href=\"https://github.com/MarvinLe/WebServer\">LC WebServer</a>.<br/>\n"
                    "\n"
                    "<p><em>Thank you for using LC WebServer.</em></p>\n"
                    "</body>\n"
                    "</html>";


class HttpServer {
public:
    enum FileState {
        FILE_OK,
        FIlE_NOT_FOUND,
        FILE_FORBIDDEN
    };

public:
    explicit HttpServer(int port = 80, const char *ip = nullptr) : serverSocket(port, ip) {
        serverSocket.bind();
        serverSocket.listen();
    }

    void run();

    void do_request(std::shared_ptr<void> arg);

private:

    void header(std::shared_ptr<HttpData>);

    FileState static_file(std::shared_ptr<HttpData>, const char *);

    void send(std::shared_ptr<HttpData>, FileState);

    void getMime(std::shared_ptr<HttpData>);
    void hanleIndex();

    ServerSocket serverSocket;
};


//#endif //WEBSERVER_SERVER_H
