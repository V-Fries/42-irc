cmake_minimum_required(VERSION 3.26)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(lib/libftpp/include)
include_directories(include)

add_executable(webserv
		lib/libftpp/include/ft_String.hpp
		lib/libftpp/include/ft.hpp
		src/main.cpp
		include/paths.hpp
		lib/libftpp/src/String/join.cpp
		lib/libftpp/src/String/split.cpp
		lib/libftpp/src/String/split.cpp
		lib/libftpp/src/String/vector_string_operators.cpp
		include/Server.hpp
		include/User.hpp
		src/User.cpp)
