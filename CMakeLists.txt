cmake_minimum_required(VERSION 3.12)
project(webserver)

set(CMAKE_CXX_STANDARD 11)
#aux_source_directory(version_0.1/test SRC_DIR)


#aux_source_directory(version_0.2/src SRC_DIR)

set(SOURCE_FILES
        version_0.2/src/httpparse.cpp
        version_0.2/src/httpresponse.cpp
        version_0.2/src/main.cpp
        version_0.2/src/server.cpp
        version_0.2/src/ssocket.cpp
        version_0.2/src/ThreadPool.cpp
        version_0.2/src/utils.cpp
        )


add_executable(webserver  ${SOURCE_FILES})