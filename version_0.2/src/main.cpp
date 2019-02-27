//
// Created by marvinle on 2019/2/1 7:26 PM.
//

#include "../include/server.h"
#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

char basePath[300] = "/Users/lichunlin/CLionProjects/webserver/";


int main(int argc, const char *argv[]) {

    //也可以将buffer作为输出参数
    if((getcwd(basePath, 300)) == NULL)
    {
        perror("getcwd error");
    }
    else
    {
        printf("%s\n", basePath);
    }

    server::HttpServer httpServer(80);
    httpServer.run();
}
