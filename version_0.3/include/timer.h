/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <deque>
#include <memory>
#include <queue>

#include "http_data.h"
#include "mutex_lock.h"

namespace csguide_webserver {

class HttpData;

class TimerNode {
public:
  TimerNode(std::shared_ptr<HttpData> httpData, size_t timeout);
  ~TimerNode();

public:
  bool IsDeleted() const { return deleted_; }

  size_t GetExpireTime() { return expired_time_; }

  bool isExpire() {
    // 平凡调用系统调用不好
    // current_time();
    return expired_time_ < current_msec_;
  }

  void Deleted();

  std::shared_ptr<HttpData> GetHttpData() { return http_data_; }

  static void CurrentTime();
private:
  static size_t current_msec_;  // 当前时间
  bool deleted_;
  size_t expired_time_;  // 毫秒
  std::shared_ptr<HttpData> http_data_;
};

struct TimerCmp {
  bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const {
    return a->GetExpireTime() > b->GetExpireTime();
  }
};

class TimerManager {
public:
  typedef std::shared_ptr<TimerNode> Shared_TimerNode;

public:
  void addTimer(std::shared_ptr<HttpData> httpData, size_t timeout);

  void handle_expired_event();

  const static size_t DEFAULT_TIME_OUT;

private:
  std::priority_queue<Shared_TimerNode, std::deque<Shared_TimerNode>, TimerCmp> timer_queue_;
  MutexLock lock_;
};

}  // namespace csguide_webserver
