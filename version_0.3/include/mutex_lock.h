/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#pragma once

#include <pthread.h>

#include "noncopyable.h"

namespace csguide_webserver {

class MutexLock : public Noncopyable {
public:
  MutexLock() { pthread_mutex_init(&mutex_, NULL); }
  ~MutexLock() { pthread_mutex_destroy(&mutex_); }
  void lock() { pthread_mutex_lock(&mutex_); }
  void unlock() { pthread_mutex_unlock(&mutex_); }
  pthread_mutex_t *getMutex() { return &mutex_; }

private:
  pthread_mutex_t mutex_;
};

class MutexLockGuard : public Noncopyable {
public:
  explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex) { mutex_.lock(); }

  ~MutexLockGuard() { mutex_.unlock(); }

private:
  MutexLock &mutex_;
};
}  // namespace csguide_webserver
