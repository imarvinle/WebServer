//
// Created by marvinle on 2019/2/1 7:26 PM.
//

#include "../include/Server.h"
#include "../include/Util.h"

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

char basePath[300] = "/Users/lichunlin/CLionProjects/webserver/";


int main(int argc, const char *argv[]) {
    if(getcwd(basePath, 300) == NULL)
    {
        perror("getcwd error");
    }
    else
    {
        printf("%s\n", basePath);
        strcat(basePath, "/version_0.3");
    }
    handle_for_sigpipe();
    HttpServer httpServer(8080);
    httpServer.run();
}
