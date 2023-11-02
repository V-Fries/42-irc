cmake_minimum_required(VERSION 3.22.1)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(lib/libftpp/include)
include_directories(include)

add_executable(webserv
		lib/libftpp/include/ft_String.hpp
		lib/libftpp/include/ft.hpp
		src/main.cpp
		lib/libftpp/src/String/join.cpp
		lib/libftpp/src/String/split.cpp
		lib/libftpp/src/String/split.cpp
		lib/libftpp/src/String/vector_string_operators.cpp
		include/ISocket.hpp
		include/Server.hpp
		include/User.hpp
		src/User.cpp
		lib/libftpp/src/String/findFirstNotOfString.cpp
		src/Server.cpp
		include/ListenSocket.hpp
		src/ListenSocket.cpp
		include/EpollEvent.hpp
		include/SignalHandler.hpp
		src/SignalHandler.cpp)
