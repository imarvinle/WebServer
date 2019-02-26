//
// Created by marvinle on 2019/2/1 7:26 PM.
//

#include "../include/server.h"

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    char *buffer;
    //也可以将buffer作为输出参数
    if((buffer = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd error");
    }
    else
    {
        printf("%s\n", buffer);
        free(buffer);
    }

    server::HttpServer httpServer(80);
    httpServer.run();
}
