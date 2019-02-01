//
// Created by marvinle on 2019/2/1 7:26 PM.
//

#include "../include/server.h"


int main(int argc, const char *argv[]) {
    server::HttpServer httpServer(80);
    httpServer.run();
}
