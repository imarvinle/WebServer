/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include "../include/thread_pool.h"

#include <pthread.h>
#include <sys/prctl.h>

#include <iostream>

namespace csguide_webserver {

ThreadPool::ThreadPool(int thread_s, int max_queue_s)
    : max_queue_size_(max_queue_s), thread_size_(thread_s), condition_(mutex_), started_(0), shutdown_(0) {
  if (thread_s <= 0 || thread_s > MAX_THREAD_SIZE) {
      thread_size_ = 4;
  }

  if (max_queue_s <= 0 || max_queue_s > MAX_QUEUE_SIZE) {
      max_queue_size_ = MAX_QUEUE_SIZE;
  }
  // 分配空间
  threads_.resize(thread_size_);

  for (int i = 0; i < thread_size_; i++) {
    // 后期可扩展出单独的Thread类，只需要该类拥有run方法即可
    if (pthread_create(&threads_[i], NULL, worker, this) != 0) {
      std::cout << "ThreadPool Init error" << std::endl;
      throw std::exception();
    }
    started_++;
  }
}

ThreadPool::~ThreadPool() {}

bool ThreadPool::Append(std::shared_ptr<void> arg, std::function<void(std::shared_ptr<void>)> fun) {
  if (shutdown_) {
    std::cout << "ThreadPool has shutdown" << std::endl;
    return false;
  }

  MutexLockGuard guard(this->mutex_);
  if (request_queue_.size() > max_queue_size_) {
    std::cout << max_queue_size_;
    std::cout << "ThreadPool too many requests" << std::endl;
    return false;
  }
  ThreadTask threadTask;
  threadTask.arg = arg;
  threadTask.process = fun;

  request_queue_.push_back(threadTask);
  //    if (request_queue.size() == 1) {
  //        condition_.notify();
  //    }
  // 之前是先判断当前队列是否为空，为空才有线程等待在上面，才需要signal
  // 而后发现其实直接signal也没事，因为signal信号就算没有等待在信号上的也没事
    condition_.Notify();
  return true;
}

void ThreadPool::Shutdown(bool graceful) {
  {
    MutexLockGuard guard(this->mutex_);
    if (shutdown_) {
      std::cout << "has shutdown" << std::endl;
    }
    shutdown_ = graceful ? graceful_mode : immediate_mode;
      condition_.NotifyAll();
  }
  for (int i = 0; i < thread_size_; i++) {
    if (pthread_join(threads_[i], NULL) != 0) {
      std::cout << "pthread_join error" << std::endl;
    }
  }
}

void *ThreadPool::worker(void *args) {
  ThreadPool *pool = static_cast<ThreadPool *>(args);
  // 退出线程
  if (pool == nullptr) return NULL;
  prctl(PR_SET_NAME, "EventLoopThread");

  // 执行线程主方法
  pool->run();
  return NULL;
}

void ThreadPool::run() {
  while (true) {
    ThreadTask requestTask;
    {
      MutexLockGuard guard(this->mutex_);
      // 无任务 且未shutdown 则条件等待, 注意此处应使用while而非if
      while (request_queue_.empty() && !shutdown_) {
          condition_.Wait();
      }

      if ((shutdown_ == immediate_mode) || (shutdown_ == graceful_mode && request_queue_.empty())) {
        break;
      }
      // FIFO
      requestTask = request_queue_.front();
      request_queue_.pop_front();
    }
    requestTask.process(requestTask.arg);
  }
}
}  // namespace csguide_webserver