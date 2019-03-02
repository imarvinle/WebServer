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

std::string basePath = "./";   //默认是程序当前目录


int main(int argc, const char *argv[]) {

    int threadNumber = 4;   //  默认线程数
    int port = 7244;        // 默认端口

    int opt;
    const char *str = "t:p:r";

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
                    if(getcwd(basePath, 300) == NULL)
                    {
                        perror("getcwd error");
                        basePath = ".";
                    }
                    else
                    {
                        printf("website root path =%s\n", basePath.c_str());
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
            default: break;
        }
    }

    handle_for_sigpipe();

    HttpServer httpServer(port);
    httpServer.run(threadNumber);
}
