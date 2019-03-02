//
// Created by marvinle on 2019/2/1 7:26 PM.
//

#include "../include/Server.h"
#include "../include/Util.h"

#include <string>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>  //for signal



std::string basePath = ".";   //默认是程序当前目录


void daemon_run()
{
    int pid;
    signal(SIGCHLD, SIG_IGN);
    //1）在父进程中，fork返回新创建子进程的进程ID；
    //2）在子进程中，fork返回0；
    //3）如果出现错误，fork返回一个负值；
    pid = fork();
    if (pid < 0)
    {
        std:: cout << "fork error" << std::endl;
        exit(-1);
    }
    //父进程退出，子进程独立运行
    else if (pid > 0)
   {
        exit(0);
    }
    //之前parent和child运行在同一个session里,parent是会话（session）的领头进程,
    //parent进程作为会话的领头进程，如果exit结束执行的话，那么子进程会成为孤儿进程，并被init收养。
    //执行setsid()之后,child将重新获得一个新的会话(session)id。
    //这时parent退出之后,将不会影响到child了。
    setsid();
    int fd;
    fd = open("/dev/null", O_RDWR, 0);
    if (fd != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
    }
    if (fd > 2)
        close(fd);
}



int main(int argc, char **argv) {

    int threadNumber = 4;   //  默认线程数
    int port = 7244;        // 默认端口
    char tempPath[256];
    int opt;
    const char *str = "t:p:r:d";
    bool daemon = false;

    while ((opt = getopt(argc, argv, str))!= -1)
    {
        switch (opt)
        {
            case 't':
            {
                threadNumber = atoi(optarg);
                break;
            }
            case 'r':
            {
                int ret = check_base_path(optarg);
                if (ret == -1) {
                    printf("Warning: \"%s\" 不存在或不可访问, 将使用当前目录作为网站根目录\n", optarg);
                    if(getcwd(tempPath, 300) == NULL)
                    {
                        perror("getcwd error");
                        basePath = ".";
                    }
                    else
                    {
                        basePath = tempPath;
                    }
                    break;
                }
                if (optarg[strlen(optarg)-1] == '/') {
                    optarg[strlen(optarg)-1] = '\0';
                }
                basePath = optarg;
                break;
            }
            case 'p':
            {
                // FIXME 端口合法性校验
                port = atoi(optarg);
                break;
            }
            case 'd':
            {
                daemon = true;
                break;
            }

            default: break;
        }
    }

    if (daemon)
        daemon_run();


    //  输出配置信息
    {
      printf("*******LC WebServer 配置信息*******\n");
      printf("端口:\t%d\n", port);
      printf("线程数:\t%d\n", threadNumber);
      printf("根目录:\t%s\n", basePath.c_str());
    }
    handle_for_sigpipe();

    HttpServer httpServer(port);
    httpServer.run(threadNumber);
}
