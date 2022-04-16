cmake_minimum_required(VERSION 3.13)
project(network)

set(CMAKE_CXX_STANDARD 11)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_definitions(-D LINUX)
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions(-D WINDOWS)
ENDIF()

add_executable(client utils.cpp client.cpp utils.h)
add_executable(server utils.cpp server.cpp utils.h)