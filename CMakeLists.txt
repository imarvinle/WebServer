cmake_minimum_required(VERSION 3.12)
project(webserver)

set(CMAKE_CXX_STANDARD 11)
#aux_source_directory(version_0.1/test SRC_DIR)


aux_source_directory(version_0.1/src SRC_DIR)

add_executable(webserver  ${SRC_DIR})