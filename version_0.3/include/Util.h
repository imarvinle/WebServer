//
// Created by marvinle on 2019/2/1 12:07 PM.
//

//#ifndef WEBSERVER_UTILS_H
//#define WEBSERVER_UTILS_H

#pragma once

#include <string>

using namespace std;

std::string &ltrim(string &);

std::string &rtrim(string &);

std::string &trim(string &);

int setnonblocking(int fd);
void handle_for_sigpipe();

int check_base_path(char *basePath);




//#endif //WEBSERVER_UTILS_H
