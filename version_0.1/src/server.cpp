//
// Created by marvinle on 2019/2/1 7:34 PM.
//

#include "../include/server.h"
#include "../include/httpparse.h"
#include "../include/httpresponse.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iostream>

using namespace server;
using namespace nsocket;
using namespace http;


void HttpServer::run() {

    while (true) {
        ClientSocket clientSocket;
        serverSocket.accept(clientSocket);
        do_request(clientSocket);
    }
}


void HttpServer::do_request(const ClientSocket & clientSocket) {

    char buffer[BUFFERSIZE];
    char header[BUFFERSIZE];
    bzero(header, BUFFERSIZE);
    bzero(buffer, BUFFERSIZE);

    sprintf(header, "HTTP/1.1 %s %s\r\n", "200", "OK");
    sprintf(header, "%sServer: LCWebServer\r\n", header);
    sprintf(header, "%sContent-type: text/html\r\n", header);
    sprintf(header, "%sConnection: close\r\n", header);

    int check_index = 0, read_index = 0, start_line = 0;
    ssize_t recv_data;
    http::HttpRequestParser::PARSE_STATE  parse_state = http::HttpRequestParser::PARSE_REQUESTLINE;

    while (true) {
        http::HttpRequest  request;

        recv_data = recv(clientSocket.fd, buffer + read_index, BUFFERSIZE - read_index, 0);
        if (recv_data == -1) {
            std::cout << "reading faild" << std::endl;
            exit(0);
        }
        if (recv_data == 0) {
            std::cout << "connection closed by peer" << std::endl;
            break;
        }

        if (recv_data > 0) {
            std::cout << "recv data" << std::endl;
            std::cout << buffer << std::endl;
        }
        read_index += recv_data;

        http::HttpRequestParser::HTTP_CODE  retcode = http::HttpRequestParser::parse_content(
                buffer, check_index, read_index, parse_state, start_line, request);
        std::cout << request << std::endl;
        if (retcode == http::HttpRequestParser::NO_REQUEST) {
            continue;
        }

        if (retcode == http::HttpRequestParser::GET_REQUEST) {
            char body[BUFFERSIZE];
            int fd = ::open("/Users/lichunlin/CLionProjects/webserver/version_0.1/index.html", O_RDONLY);
            int bodysize = ::read(fd, body, BUFFERSIZE);
            sprintf(header, "%sContent-length: %d\r\n\r\n", header, (int)strlen(body));
            ::send(clientSocket.fd, header, strlen(header), 0);
            ::send(clientSocket.fd, body, strlen(body), 0);
            close(fd);
            break;
        } else {
            std::cout << retcode << std::endl;
            char *body = "404 Not Found";
            sprintf(header, "%sContent-length: %d\r\n\r\n", header, (int)strlen(body));
            ::send(clientSocket.fd, header, strlen(header), 0);
            ::send(clientSocket.fd, body, strlen(body), 0);
            break;
        }
    }
}

void HttpServer::header(const HttpRequest &request,  HttpResponse &response) {

    response.addHeader("Server", "LC WebServer");

}

void HttpServer::getMime(const http::HttpRequest &request, http::HttpResponse &response) {

}
