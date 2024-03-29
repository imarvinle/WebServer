/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <pthread.h>

#include "mutex_lock.h"
#include "noncopyable.h"

namespace csguide_webserver {

class Condition : public Noncopyable {
public:
  explicit Condition(MutexLock &mutex) : mutex_(mutex) { pthread_cond_init(&cond_, NULL); }

  ~Condition() { pthread_cond_destroy(&cond_); }

  void inline Wait() { pthread_cond_wait(&cond_, mutex_.GetMutex()); }

  void inline Notify() { pthread_cond_signal(&cond_); }

  void inline NotifyAll() { pthread_cond_broadcast(&cond_); }

private:
  MutexLock &mutex_;
  pthread_cond_t cond_;
};
}  // namespace csguide_webserver