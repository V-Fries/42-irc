cmake_minimum_required(VERSION 3.26)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(lib/libftpp/include)

add_executable(webserv
		lib/libftpp/include/String.hpp
		lib/libftpp/src/String.cpp
		src/main.cpp)
