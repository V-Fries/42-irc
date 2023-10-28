cmake_minimum_required(VERSION 3.26)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(lib/libftpp/include)
include_directories(include)

add_compile_options(-Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address)

add_link_options(-g3 -fsanitize=address)

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
		src/RegisterSocket.cpp)
