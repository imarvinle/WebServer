/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle) 
 */

#include <iostream>
#include <string>

#include "../include/Util.h"

using namespace std;

using namespace 

void test_ltrim() {
  std::string s = "  \thelloworld\t ";
  util::ltrim(s);
  if (s == "helloworld\t ") {
    cout << "ltrim ok" << endl;
  } else {
    cout << "test ltrim faild" << endl;
  }
}
void test_rtrim() {
  std::string s = "  \thelloworld\t ";
  util::rtrim(s);
  if (s == "  \thelloworld") {
    cout << "rtrim ok" << endl;
  } else {
    cout << "test rtrim faild" << endl;
  }
}

void test_trim() {
  std::string s = " 1";
  util::trim(s);
  if (s == "1") {
    cout << "trim ok" << endl;
  } else {
    cout << "test trim faild" << endl;
  }
}

int main() {
  test_ltrim();
  test_rtrim();
  test_trim();
}
