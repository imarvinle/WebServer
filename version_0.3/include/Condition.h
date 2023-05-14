/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle) 
 */


#pragma once

#include <pthread.h>

#include "MutexLock.h"
#include "noncopyable.h"

class Condition : public noncopyable {
  public:
  explicit Condition(MutexLock &mutex) : mutex_(mutex) {
    pthread_cond_init(&cond_, NULL);
  }
  ~Condition() { pthread_cond_destroy(&cond_); }

  void wait() { pthread_cond_wait(&cond_, mutex_.getMutex()); }

  void notify() { pthread_cond_signal(&cond_); }

  void notifyAll() { pthread_cond_broadcast(&cond_); }

  private:
  MutexLock &mutex_;
  pthread_cond_t cond_;
};
