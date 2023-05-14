//
// Created by marvinle on 2019/2/26 9:42 AM.
//

#include "../include/ThreadPool.h"

#include <pthread.h>

#include <iostream>

using namespace thread;

ThreadPool::ThreadPool(int thread_s, int max_queue_s)
    : max_queue_size(max_queue_s),
      thread_size(thread_s),
      condition_(mutex_),
      started(0),
      shutdown_(0) {
  if (thread_s <= 0 || thread_s > MAX_THREAD_SIZE) {
    thread_size = 4;
  }

  if (max_queue_s <= 0 || max_queue_s > MAX_QUEUE_SIZE) {
    max_queue_size = MAX_QUEUE_SIZE;
  }
  // 分配空间
  threads.resize(thread_size);

  for (int i = 0; i < thread_size; i++) {
    // 后期可扩展出单独的Thread类，只需要该类拥有run方法即可

    if (pthread_create(&threads[i], NULL, worker, this) != 0) {
      std::cout << "ThreadPool init error" << std::endl;
      throw std::exception();
    }
    started++;
  }
}

ThreadPool::~ThreadPool() {}

bool ThreadPool::append(ThreadTask *request) {
  if (request == nullptr) return false;

  if (shutdown_) {
    std::cout << "ThreadPool has shutdown" << std::endl;
    return false;
  }

  MutexLockGuard guard(this->mutex_);
  if (request_queue.size() > max_queue_size) {
    std::cout << max_queue_size;
    std::cout << "ThreadPool too many requests" << std::endl;
    return false;
  }
  request_queue.push_back(request);
  if (request_queue.size() == 1) {
    condition_.notify();
  }
  return true;
}

void ThreadPool::shutdown(bool graceful) {
  {
    MutexLockGuard guard(this->mutex_);
    if (shutdown_) {
      std::cout << "has shutdown" << std::endl;
    }
    shutdown_ = graceful ? graceful_mode : immediate_mode;
    condition_.notifyAll();
  }
  for (int i = 0; i < thread_size; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      std::cout << "pthread_join error" << std::endl;
    }
  }
}

void *ThreadPool::worker(void *args) {
  ThreadPool *pool = static_cast<ThreadPool *>(args);
  // 退出线程
  if (pool == nullptr) return NULL;
  // 执行线程主方法
  pool->run();
  return NULL;
}

void ThreadPool::run() {
  while (true) {
    ThreadTask *request = nullptr;
    {
      MutexLockGuard guard(this->mutex_);
      // 无任务 且未shutdown 则条件等待, 注意此处应使用while而非if
      while (request_queue.empty() && !shutdown_) {
        condition_.wait();
      }

      if ((shutdown_ == immediate_mode) ||
          (shutdown_ == graceful_mode && request_queue.empty())) {
        break;
      }
      // FIFO
      request = request_queue.front();
      request_queue.pop_front();
    }
    if (request == nullptr) continue;

    request->process(request->arg);
    delete request;
  }
}
