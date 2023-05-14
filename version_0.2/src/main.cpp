/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle) 
 */

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "../include/server.h"

char basePath[300] = "/Users/lichunlin/CLionProjects/webserver/";

int main(int argc, const char *argv[]) {
  //也可以将buffer作为输出参数
  if ((getcwd(basePath, 300)) == NULL) {
    perror("getcwd error");
  } else {
    printf("%s\n", basePath);
  }

  server::HttpServer httpServer(80);
  httpServer.run();
}
