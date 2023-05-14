/*
 * Copyright (C) 2019 CSGuide(https://csguide.cn)
 * Author: xiaobei (https://github.com/imarvinle)
 */

#include "../include/epoll.h"

#include <sys/epoll.h>

#include <cstdio>
#include <iostream>
#include <vector>

#include "../include/util.h"

namespace csguide_webserver {

std::unordered_map<int, std::shared_ptr<HttpData>> Epoll::http_data_map_;
const int Epoll::MAX_EVENTS = 10000;
epoll_event *Epoll::events_;

// 可读 | ET模 | 保证一个socket连接在任一时刻只被一个线程处理
const __uint32_t Epoll::DEFAULT_EVENTS = (EPOLLIN | EPOLLET | EPOLLONESHOT);

TimerManager Epoll::timer_manager_;

int Epoll::Init(int max_events) {
  int epoll_fd = ::epoll_create(max_events);
  if (epoll_fd == -1) {
    std::cout << "epoll create error" << std::endl;
    exit(-1);
  }
    events_ = new epoll_event[max_events];
  return epoll_fd;
}

int Epoll::Addfd(int epoll_fd, int fd, __uint32_t events, std::shared_ptr<HttpData>) {
  epoll_event event;
  event.events = (EPOLLIN | EPOLLET);
  event.data.fd = fd;
  // 增加httpDataMap
  http_data_map_[fd] = httpData;
  int ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
  if (ret < 0) {
    std::cout << "epoll add error" << endl;
    // 释放httpData
    http_data_map_[fd].reset();
    return -1;
  }
  return 0;
}

int Epoll::Modfd(int epoll_fd, int fd, __uint32_t events, std::shared_ptr<HttpData>) {
  epoll_event event;
  event.events = events;
  event.data.fd = fd;
  // 每次更改的时候也更新 httpDataMap
  http_data_map_[fd] = httpData;
  int ret = ::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
  if (ret < 0) {
    std::cout << "epoll mod error" << endl;
    // 释放httpData
    http_data_map_[fd].reset();
    return -1;
  }
  return 0;
}

int Epoll::Delfd(int epoll_fd, int fd, __uint32_t events) {
  epoll_event event;
  event.events = events;
  event.data.fd = fd;
  int ret = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
  if (ret < 0) {
    std::cout << "epoll del error" << endl;
    return -1;
  }
  auto it = http_data_map_.find(fd);
  if (it != http_data_map_.end()) {
    http_data_map_.erase(it);
  }
  return 0;
}

void Epoll::HandleConnection(const ServerSocket &serverSocket) {
  std::shared_ptr<ClientSocket> tempClient(new ClientSocket);
  // epoll 是ET模式，循环接收连接
  // 需要将listen_fd设置为non-blocking

  while (serverSocket.accept(*tempClient) > 0) {
    // 设置非阻塞
    int ret = setnonblocking(tempClient->fd);
    if (ret < 0) {
      std::cout << "setnonblocking error" << std::endl;
      tempClient->close();
      continue;
    }

    // FIXME 接受新客户端 构造HttpData并添加定时器

    // 在这里做限制并发, 暂时未完成

    std::shared_ptr<HttpData> sharedHttpData(new HttpData);
    sharedHttpData->request_ = std::shared_ptr<HttpRequest>(new HttpRequest());
    sharedHttpData->response_ = std::shared_ptr<HttpResponse>(new HttpResponse());

    std::shared_ptr<ClientSocket> sharedClientSocket(new ClientSocket());
    sharedClientSocket.swap(tempClient);
    sharedHttpData->client_socket_ = sharedClientSocket;
    sharedHttpData->epoll_fd = serverSocket.epoll_fd;

      Addfd(serverSocket.epoll_fd, sharedClientSocket->fd, DEFAULT_EVENTS, sharedHttpData);
    // FIXME 默认超时时间5 秒测试添加定时器
    timer_manager_.addTimer(sharedHttpData, TimerManager::DEFAULT_TIME_OUT);
  }
}

std::vector<std::shared_ptr<HttpData>> Epoll::Poll(const ServerSocket &serverSocket, int max_event, int timeout) {
  int event_num = epoll_wait(serverSocket.epoll_fd, events_, max_event, timeout);
  if (event_num < 0) {
    std::cout << "epoll_num=" << event_num << std::endl;
    std::cout << "epoll_wait error" << std::endl;
    std::cout << errno << std::endl;
    exit(-1);
  }

  std::vector<std::shared_ptr<HttpData>> httpDatas;
  // 遍历events集合
  for (int i = 0; i < event_num; i++) {
    int fd = events_[i].data.fd;

    // 监听描述符
    if (fd == serverSocket.listen_fd) {
        HandleConnection(serverSocket);
    } else {
      // 出错的描述符，移除定时器， 关闭文件描述符
      if ((events_[i].events & EPOLLERR) || (events_[i].events & EPOLLRDHUP) || (events_[i].events & EPOLLHUP)) {
        auto it = http_data_map_.find(fd);
        if (it != http_data_map_.end()) {
          // 将HttpData节点和TimerNode的关联分开，这样HttpData会立即析构，在析构函数内关闭文件描述符等资源
            it->second->CloseTimer();
          // httpDataMap.erase(it);
        }
        continue;
      }

      auto it = http_data_map_.find(fd);
      if (it != http_data_map_.end()) {
        if ((events_[i].events & EPOLLIN) || (events_[i].events & EPOLLPRI)) {
          httpDatas.push_back(it->second);
          // std::cout << "定时器中找到:" << fd << std::endl;
          // 清除定时器 HttpData.closeTimer()
            it->second->CloseTimer();
          http_data_map_.erase(it);
        }
      } else {
        std::cout << "长连接第二次连接未找到" << std::endl;
        ::close(fd);
        continue;
      }
      // 这里有个问题是 TimerNode正常超时释放时
    }
  }
  return httpDatas;
}

}  // namespace csguide_webserver