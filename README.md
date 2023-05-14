# A C++ Lightweight Web Server


[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.org/MarvinLe/WebServer.svg?branch=master)](https://travis-ci.org/MarvinLe/WebServer)


## 简介

这是一个轻量级的Web服务器，目前支持GET、HEAD方法处理静态资源。并发模型选择: 单进程＋Reactor+非阻塞方式运行。

测试页面: [http://csguide.cn:8080/](http://csguide.cn:8080/)


---

| Part Ⅰ | Part Ⅱ |
| :---------: | :---------: |
|  [整体设计](https://github.com/MarvinLe/WebServer/blob/master/%E6%95%B4%E4%BD%93%E8%AE%BE%E8%AE%A1.md)| [性能测试分析](https://github.com/MarvinLe/WebServer/blob/master/%E6%80%A7%E8%83%BD%E6%B5%8B%E8%AF%95%E5%88%86%E6%9E%90.md) |

---

## 开发部署环境

+ 操作系统: Ubuntu 16.04

+ 编译器: g++ 5.4

+ 版本控制: git

+ 自动化构建: cmake

+ 集成开发工具: CLion

+ 编辑器: Vim

+ 压测工具：[WebBench](https://github.com/EZLippi/WebBench)



## Usage

```
cmake . && make 

./webserver [-p port] [-t thread_numbers]  [-r website_root_path] [-d daemon_run]

```

## 核心功能及技术

+ 状态机解析HTTP请求，目前支持 HTTP GET、HEAD方法

+ 添加定时器支持HTTP长连接，定时回调handler处理超时连接

+ 使用 priority queue 实现的最小堆结构管理定时器，使用标记删除，以支持惰性删除，提高性能

+ 使用epoll + 非阻塞IO + 边缘触发(ET) 实现高并发处理请求，使用Reactor编程模型

+ epoll使用EPOLLONESHOT保证一个socket连接在任意时刻都只被一个线程处理

+ 使用线程池提高并发度，并降低频繁创建线程的开销
+ 同步互斥的介绍

+ 使用RAII手法封装互斥器(pthrea_mutex_t)、 条件变量(pthread_cond_t)等线程同步互斥机制，使用RAII管理文件描述符等资源

+ 使用shared_ptr、weak_ptr管理指针，防止内存泄漏

  

## 开发计划
+ 添加异步日志系统，记录服务器运行状态
+ 增加json配置文件，支持类似nginx的多网站配置
+ 提供CGI支持
+ 类似nginx的反向代理和负载均衡
+ 必要时增加可复用内存池
