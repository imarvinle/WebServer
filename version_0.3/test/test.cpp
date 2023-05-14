/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <string>

using namespace std;

int main() {
  //    string s = "/helo/world/index?word=12";
  //    s.erase(s.rfind('?'));
  //    string filetype;
  //    if (s.rfind('.') != string::npos){
  //        filetype = s.substr(s.rfind('.'));
  //    }
  //
  //
  //    cout << s << endl;
  //    if (filetype != "") {
  //        cout << filetype << endl;
  //    } else {
  //        cout << " no file type" << endl;
  //    }
  char *buffer;
  //也可以将buffer作为输出参数
  if ((buffer = getcwd(NULL, 0)) == NULL) {
    perror("getcwd error");
  } else {
    printf("%s\n", buffer);
    free(buffer);
  }

  enum Code { ok = 201, notfound = 404 };

  Code code = ok;
  printf("%d\n", code);
}