/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

namespace csguide_webserver {
class Noncopyable {
  public:
  Noncopyable(const Noncopyable&) = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;

  protected:
  Noncopyable() = default;
  ~Noncopyable() = default;
};
}  // namespace csguide_webserver
