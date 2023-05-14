/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle) 
 */
#ifndef WEBSERVER_UTILS_H
#define WEBSERVER_UTILS_H

#include <string>

namespace util {
using namespace std;
std::string& ltrim(string&);
std::string& rtrim(string&);
std::string& trim(string&);
}  // namespace util

#endif  // WEBSERVER_UTILS_H
