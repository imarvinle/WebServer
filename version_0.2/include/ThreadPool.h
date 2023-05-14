/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle) 
 */

#ifndef WEBSERVER_THREADPOLL_H
#define WEBSERVER_THREADPOLL_H

#include <pthread.h>

#include <functional>
#include <list>
#include <vector>

#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

namespace thread {
const int MAX_THREAD_SIZE = 1024;
const int MAX_QUEUE_SIZE = 10000;

typedef enum { immediate_mode = 1, graceful_mode = 2 } ShutdownMode;

struct ThreadTask {
  std::function<void(void*)> process;
  void* arg;
};

class ThreadPool {
  public:
  ThreadPool(int thread_s, int max_queue_s);
  ~ThreadPool();
  bool append(thread::ThreadTask* request);
  void shutdown(bool graceful);

  private:
  static void* worker(void* args);
  void run();

  private:
  // 线程同步互斥, mutex_ 在 condition_前面
  MutexLock mutex_;
  Condition condition_;

  // 线程池属性
  int thread_size;
  int max_queue_size;
  int started;
  int shutdown_;
  std::vector<pthread_t> threads;
  std::list<thread::ThreadTask*> request_queue;
};

}  // namespace thread

#endif  // WEBSERVER_THREADPOLL_H
