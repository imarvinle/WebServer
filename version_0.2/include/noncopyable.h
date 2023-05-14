/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle) 
 */
#ifndef WEBSERVER_NONCOPYABLE_H
#define WEBSERVER_NONCOPYABLE_H

class noncopyable {
  public:
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;

  protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
#endif  // WEBSERVER_NONCOPYABLE_H
