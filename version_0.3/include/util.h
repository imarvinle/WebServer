/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <string>

using namespace std;

namespace csguide_webserver {

std::string &ltrim(string &);

std::string &rtrim(string &);

std::string &trim(string &);

int setnonblocking(int fd);

void handle_for_sigpipe();

int check_base_path(char *basePath);

bool endsWith(std::string const &str, std::string const &suffix);

}  // namespace csguide_webserver