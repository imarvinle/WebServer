/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include <signal.h>  //for signal
#include <stdio.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

#include "../include/logger.h"
#include "../include/server.h"
#include "../include/util.h"

using namespace csguide_webserver;


void daemon_run() {
  int pid;
  signal(SIGCHLD, SIG_IGN);
  // 1）在父进程中，fork返回新创建子进程的进程ID；
  // 2）在子进程中，fork返回0；
  // 3）如果出现错误，fork返回一个负值；
  pid = fork();
  if (pid < 0) {
    std::cout << "fork error" << std::endl;
    exit(-1);
  }
  //父进程退出，子进程独立运行
  else if (pid > 0) {
    exit(0);
  }
  //之前parent和child运行在同一个session里,parent是会话（session）的领头进程,
  // parent进程作为会话的领头进程，如果exit结束执行的话，那么子进程会成为孤儿进程，并被init收养。
  //执行setsid()之后,child将重新获得一个新的会话(session)id。
  //这时parent退出之后,将不会影响到child了。
  setsid();
  int fd;
  fd = open("/dev/null", O_RDWR, 0);
  if (fd != -1) {
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
  }
  if (fd > 2) close(fd);
}

void CheckConf(ServerConf &server_conf) {
  int ret = CheckBasePath(server_conf.root);
  if (ret != 0) {
    Logger::GetInstance().LogRun("Warning: \"%s\" 不存在或不可访问, 将使用当前目录作为网站根目录", optarg);
    char CurrenctPath[256];
    // 获取当前目录出错，使用 . 代替
    if (getcwd(CurrenctPath, 300) == NULL) {
      Logger::GetInstance().LogErr("getcwd err");
      server_conf.root = ".";
    } else {
      server_conf.root = CurrenctPath;
    }
  }
  // 去除最后的 / ，因为 basepath + url，url部分有 /
  if (server_conf.root[server_conf.root.size() - 1] == '/') {
    server_conf.root.pop_back();
  }
}

int main(int argc, char **argv) {
  int opt;
  const char *str = "f:";

  ServerConf server_conf;

  while ((opt = getopt(argc, argv, str)) != -1) {
    switch (opt) {
      case 'f': {
        if (optarg != NULL) {
          int ret = ParseConfig(optarg, server_conf);
          if (0 != ret) {
            Logger::GetInstance().LogErr("ParseConfig Err, exit");
            exit(ret);
          }
        } else {
          Logger::GetInstance().LogErr("empty opt: f");
        }
        break;
      }
    }
  }

  CheckConf(server_conf);

  //  输出配置信息
  {
    Logger::GetInstance().LogRun("*******CSGuide WebServer 配置信息*******");
    Logger::GetInstance().LogRun("端口:\t%d", server_conf.port);
    Logger::GetInstance().LogRun("线程数:\t%d", server_conf.thread_num);
    Logger::GetInstance().LogRun("根目录:\t%s", server_conf.root.c_str());
    Logger::GetInstance().LogRun("守护模式:\t%s", server_conf.daemon ? "true" : "false");
  }

  if (server_conf.daemon) daemon_run();

  HandleForSigPipe();

  HttpServer httpServer(server_conf);
  httpServer.Run();
}
