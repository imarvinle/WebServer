//
// Created by marvinle on 2019/2/1 12:11 PM.
//

#include "../include/Util.h"

#include <string>
#include <iostream>

using namespace std;

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

