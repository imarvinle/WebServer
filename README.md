# A C++ Lightweight Web Server


[![license](https://img.shields.io/github/license/mashape/apistatus.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.org/MarvinLe/WebServer.svg?branch=master)](https://travis-ci.org/MarvinLe/WebServer)


## 简介

这是一个轻量级的Web服务器，目前支持GET、HEAD方法处理静态资源。并发模型选择: 单进程＋Reactor+非阻塞方式运行。

测试页面: [http://marvinle.cn:8080/](http://marvinle.cn:8080/)


---

| Part Ⅰ | Part Ⅱ | Part Ⅲ | Part Ⅳ | Part Ⅴ | Part Ⅵ | Part Ⅷ | Part Ⅸ | Part Ⅹ |
| :--------: | :---------: | :---------: | :---------: | :---------: | :---------: |:--------:| :--------:|:--------:|
|  [项目目的](https://github.com/linw7/TKeed/blob/master/%E9%A1%B9%E7%9B%AE%E7%9B%AE%E7%9A%84.md)  | [并发模型](https://github.com/linw7/TKeed/blob/master/%E5%B9%B6%E5%8F%91%E6%A8%A1%E5%9E%8B.md)|[核心结构](https://github.com/linw7/TKeed/blob/master/%E6%A0%B8%E5%BF%83%E7%BB%93%E6%9E%84%E4%BD%93.md)|[整体架构](https://github.com/linw7/TKeed/blob/master/%E6%9E%B6%E6%9E%84%E5%88%86%E6%9E%90.md)|  [主要函数](https://github.com/linw7/TKeed/blob/master/%E4%B8%BB%E8%A6%81%E5%87%BD%E6%95%B0.md)| [遇到的困难](https://github.com/linw7/TKeed/blob/master/%E5%90%AF%E7%A4%BA%E5%BD%95.md) |  [测试及改进](https://github.com/linw7/TKeed/blob/master/%E6%B5%8B%E8%AF%95%E5%8F%8A%E6%94%B9%E8%BF%9B.md) | [背景知识](https://github.com/linw7/TKeed/blob/master/%E8%83%8C%E6%99%AF%E7%9F%A5%E8%AF%86.md)|[使用教程](https://asciinema.org/a/132577)|

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