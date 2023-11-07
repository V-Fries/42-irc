cmake_minimum_required(VERSION 3.22.1)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_compile_options(-Wall -Wextra -Werror -g3)

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
		src/User/User.cpp
		lib/libftpp/src/String/findFirstNotOfString.cpp
		src/Server.cpp
		include/ListenSocket.hpp
		src/ListenSocket.cpp
		include/SignalHandler.hpp
		src/SignalHandler.cpp
		lib/libftpp/src/String/getFirstWord.cpp
		lib/libftpp/src/String/getFirstWord.cpp
		lib/libftpp/src/String/skipChars.cpp
		src/User/_handlePASS.cpp
		src/User/_handleUSER.cpp
		src/User/_handleNICK.cpp
		include/NumericReplies.hpp
		src/NumericReplies.cpp
		include/Channel.hpp
		src/Channel.cpp
		src/Command.cpp
		lib/libftpp/include/ft_Set.hpp
		lib/libftpp/include/ft_Set.hpp
		lib/libftpp/include/ft_Exception.hpp
		lib/libftpp/src/Exception.cpp
		lib/libftpp/src/Log.cpp
		lib/libftpp/include/ft_Log.hpp
		"lib/libftpp/include/ft_Io.hpp"
		lib/libftpp/src/Io.cpp)
