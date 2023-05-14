/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

namespace csguide_webserver {
class noncopyable {
  public:
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;

  protected:
  noncopyable() = default;
  ~noncopyable() = default;
};
}  // namespace csguide_webserver
