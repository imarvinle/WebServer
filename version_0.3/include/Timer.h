//
// Created by marvinle on 2019/2/26 3:19 PM.
//
//#ifndef WEBSERVER_TIMER_H
//#define WEBSERVER_TIMER_H

#pragma once

#include "HttpData.h"
#include "MutexLock.h"

#include <queue>
#include <deque>
#include <memory>



class HttpData;

class TimerNode {
public:

    TimerNode(std::shared_ptr<HttpData> httpData, size_t timeout);
    ~TimerNode();
public:
    bool isDeleted() const { return deleted_; }

    size_t getExpireTime() { return expiredTime_; }

    bool isExpire() {
        // 平凡调用系统调用不好
        //current_time();
        return expiredTime_ < current_msec;
    }

    void deleted();

    std::shared_ptr<HttpData> getHttpData() { return httpData_; }

    static void current_time();

    static size_t current_msec; // 当前时间

private:
    bool deleted_;
    size_t expiredTime_;    // 毫秒
    std::shared_ptr<HttpData> httpData_;
};

struct TimerCmp {
    bool operator()(std::shared_ptr<TimerNode> &a, std::shared_ptr<TimerNode> &b) const {
        return a->getExpireTime() > b->getExpireTime();
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
    std::priority_queue<Shared_TimerNode, std::deque<Shared_TimerNode>, TimerCmp> TimerQueue;
    MutexLock lock_;
};


//#endif //WEBSERVER_TIMER_H
