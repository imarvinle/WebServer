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

int main(int argc, const char *argv[]) {
  char *buffer;
  //也可以将buffer作为输出参数
  if ((buffer = getcwd(NULL, 0)) == NULL) {
    perror("getcwd error");
  } else {
    printf("%s\n", buffer);
    free(buffer);
  }

  server::HttpServer httpServer(80);
  httpServer.run();
}
