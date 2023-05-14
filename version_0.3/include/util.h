/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <string>

#include "server.h"

using namespace std;

namespace csguide_webserver {

std::string &Ltrim(string &);

std::string &Rtrim(string &);

std::string &Trim(string &);

int SetNonBlocking(int fd);

void HandleForSigPipe();

int CheckBasePath(const std::string &base_path);

bool EndsWith(std::string const &str, std::string const &suffix);

int StrToInt(const std::string &str, int &result);

int ParseConfig(const std::string config_file, ServerConf &server_conf);

std::string ExtractSubstring(const std::string &input, char start_char, char end_char);

}  // namespace csguide_webserver