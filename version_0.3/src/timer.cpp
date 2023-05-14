/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include "../include/timer.h"

#include <sys/time.h>
#include <unistd.h>

#include "../include/epoll.h"

namespace csguide_webserver {

size_t TimerNode::current_msec_ = 0;  // 当前时间

const size_t TimerManager::DEFAULT_TIME_OUT = 20 * 1000;  // 20s

TimerNode::TimerNode(std::shared_ptr<HttpData> httpData, size_t timeout) : deleted_(false), http_data_(httpData) {
    CurrentTime();
    expired_time_ = current_msec_ + timeout;
}

TimerNode::~TimerNode() {
  // FIXME
  // 析构关闭资源的时候，要讲httpDataMap中的引用,否则资源无法关闭，后期可改进为httpDataMap存储
  // weak_ptr<HttpData> std::cout << "TimerNode析构" << std::endl;
  // 析构时如果是被deleted 则httpData为NULL,
  // 不用处理，而如果是超时，则需要删除Epoll中的httpDataMap中
  if (http_data_) {
    auto it = Epoll::http_data_map_.find(http_data_->client_socket_->fd_);
    if (it != Epoll::http_data_map_.end()) {
      Epoll::http_data_map_.erase(it);
    }
  }
}

void inline TimerNode::CurrentTime() {
  struct timeval cur;
  gettimeofday(&cur, NULL);
    current_msec_ = (cur.tv_sec * 1000) + (cur.tv_usec / 1000);
}

void TimerNode::Deleted() {
  // 删除采用标记删除， 并及时析构HttpData，以关闭描述符
  // 关闭定时器时应该把 httpDataMap 里的HttpData 一起erase
  http_data_.reset();
  deleted_ = true;
}

void TimerManager::addTimer(std::shared_ptr<HttpData> httpData, size_t timeout) {
  Shared_TimerNode timerNode(new TimerNode(httpData, timeout));
  {
    MutexLockGuard guard(lock_);
    timer_queue_.push(timerNode);
    // 将TimerNode和HttpData关联起来
      httpData->SetTimer(timerNode);
  }
}

void TimerManager::handle_expired_event() {
  MutexLockGuard guard(lock_);
  // 更新当前时间
  // std::cout << "开始处理超时事件" << std::endl;
    TimerNode::CurrentTime();
  while (!timer_queue_.empty()) {
    Shared_TimerNode timerNode = timer_queue_.top();
    if (timerNode->IsDeleted()) {
      // 删除节点
      timer_queue_.pop();
    } else if (timerNode->isExpire()) {
      // 过期 删除
      timer_queue_.pop();
    } else {
      break;
    }
  }
}

}  // namespace csguide_webserver