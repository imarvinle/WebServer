/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <string>

using namespace std;

namespace csguide_webserver {

std::string &Ltrim(string &);

std::string &Rtrim(string &);

std::string &Trim(string &);

int SetNonBlocking(int fd);

void HandleForSigPipe();

int CheckBasePath(char *base_path);

bool EndsWith(std::string const &str, std::string const &suffix);

}  // namespace csguide_webserver