/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 * Brief: 定义不可复制（Non-copyable）类，禁用了拷贝构造函数和拷贝赋值运算符
 *        通过将构造和析构函数声明为protected，允许子类继承并实例化。
 */

#pragma once

namespace csguide_webserver {
class Noncopyable {
public:
  // 禁用拷贝构造函数
  Noncopyable(const Noncopyable&) = delete;

  // 禁用拷贝赋值运算符
  Noncopyable& operator=(const Noncopyable&) = delete;

protected:
  // 默认构造函数，子类可以实例化
  Noncopyable() = default;

  // 默认析构函数
  ~Noncopyable() = default;
};
}  // namespace csguide_webserver
