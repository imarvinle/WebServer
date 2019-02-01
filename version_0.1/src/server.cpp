//
// Created by marvinle on 2019/2/1 7:34 PM.
//

#include "../include/server.h"
#include "../include/httpparse.h"

#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>


server::HttpServer::HttpServer(int port, const char *ip): serverSocket(port, ip){
    serverSocket.bind();
    serverSocket.listen();
}

void server::HttpServer::run() {
    char buffer[BUFFERSIZE];
    char header[BUFFERSIZE];


    while (true) {
        nsocket::ClientSocket clientSocket;
        bzero(header, BUFFERSIZE);
        bzero(buffer, BUFFERSIZE);

        sprintf(header, "HTTP/1.1 %s %s\r\n", "200", "OK");
        sprintf(header, "%sServer: LCWebServer\r\n", header);
        sprintf(header, "%sContent-type: text/html\r\n", header);
        sprintf(header, "%sConnection: close\r\n", header);

        serverSocket.accept(clientSocket);
        int check_index = 0, read_index = 0, recv_data = 0, start_line = 0;
        parse::HttpRequestParser::PARSE_STATE  parse_state = parse::HttpRequestParser::PARSE_REQUESTLINE;

        while (true) {
            parse::HttpRequest  request;

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

            parse::HttpRequestParser::HTTP_CODE  retcode = parse::HttpRequestParser::parse_content(
                    buffer, check_index, read_index, parse_state, start_line, request);
            if (retcode == parse::HttpRequestParser::NO_REQUEST) {
                continue;
            }

            if (retcode == parse::HttpRequestParser::GET_REQUEST) {
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
}