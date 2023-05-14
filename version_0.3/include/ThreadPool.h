//
// Created by marvinle on 2019/2/25 11:26 PM.
//

//#ifndef WEBSERVER_THREADPOLL_H
//#define WEBSERVER_THREADPOLL_H

#pragma once

#include <pthread.h>

#include <functional>
#include <list>
#include <memory>
#include <vector>

#include "Condition.h"
#include "MutexLock.h"
#include "noncopyable.h"

const int MAX_THREAD_SIZE = 1024;
const int MAX_QUEUE_SIZE = 10000;

typedef enum { immediate_mode = 1, graceful_mode = 2 } ShutdownMode;

struct ThreadTask {
  std::function<void(std::shared_ptr<void>)>
      process;                // 实际传入的是Server::do_request;
  std::shared_ptr<void> arg;  // 实际应该是HttpData对象
};

class ThreadPool {
  public:
  ThreadPool(int thread_s, int max_queue_s);

  ~ThreadPool();

  bool append(std::shared_ptr<void> arg,
              std::function<void(std::shared_ptr<void>)> fun);

  void shutdown(bool graceful);

  private:
  static void *worker(void *args);

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
  std::list<ThreadTask> request_queue;
};

//#endif //WEBSERVER_THREADPOLL_H
