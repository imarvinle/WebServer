//
// Created by marvinle on 2019/2/25 3:29 PM.
//

#pragma once

//#ifndef WEBSERVER_NONCOPYABLE_H
//#define WEBSERVER_NONCOPYABLE_H

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;

};
//#endif //WEBSERVER_NONCOPYABLE_H
