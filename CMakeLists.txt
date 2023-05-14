cmake_minimum_required(VERSION 3.5)
project(webserver)

set(CMAKE_CXX_STANDARD 11)
#aux_source_directory(version_0.1/test SRC_DIR)


#aux_source_directory(version_0.3/src SRC_DIR)

#set(SOURCE_FILES
#        version_0.2/src/httpparse.cpp
#        version_0.2/src/httpresponse.cpp
#        version_0.2/src/main.cpp
#        version_0.2/src/server.cpp
#        version_0.2/src/ssocket.cpp
#        version_0.2/src/ThreadPool.cpp
#        version_0.2/src/Util.cpp
#        )


set(CXX_FLAGS
        -Wall
        -std=c++11
        -lpthread
        -Wno-unused-parameter
        -O3
        )

link_libraries(pthread)

set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O3")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")


set(SOURCE_FILES
        version_0.3/src/http/http_request.cpp
        version_0.3/src/http/http_data.cpp
        version_0.3/src/http/http_parse.cpp
        version_0.3/src/http/http_response.cpp
        version_0.3/src/http/server.cpp
        version_0.3/src/socket.cpp
        version_0.3/src/thread_pool.cpp
        version_0.3/src/timer.cpp
        version_0.3/src/util.cpp
        version_0.3/src/epoll.cpp
        version_0.3/src/main.cpp
        )

add_executable(webserver  ${SOURCE_FILES})