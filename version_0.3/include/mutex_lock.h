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

  void inline Lock() { pthread_mutex_lock(&mutex_); }

  void inline Unlock() { pthread_mutex_unlock(&mutex_); }

  pthread_mutex_t *GetMutex() { return &mutex_; }

private:
  pthread_mutex_t mutex_;
};

class MutexLockGuard : public Noncopyable {
public:
  explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex) { mutex_.Lock(); }

  ~MutexLockGuard() { mutex_.Unlock(); }

private:
  MutexLock &mutex_;
};
}  // namespace csguide_webserver
