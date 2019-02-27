//
// Created by marvinle on 2019/2/27 10:19 AM.
//

#include "../../include/HttpData.h"




void HttpData::closeTimer() {
    // 首先判断Timer是否还在， 有可能已经超时释放
    if (timer_.lock())
    {
        std::shared_ptr<TimerNode> tempTimer(timer_.lock());
        tempTimer->deleted();
        // 断开weak_ptr
        timer_.reset();
    }
}

void HttpData::setTimer(std::shared_ptr<TimerNode> timer) {
    timer_  = timer;
}

