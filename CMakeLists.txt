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
		lib/libftpp/include/ft_Io.hpp
		lib/libftpp/src/Io.cpp
		src/User/_handlePRIVMSG.cpp
		lib/libftpp/src/String/toLower.cpp
		src/User/_handleJOIN.cpp
		src/User/_handlePING.cpp
		src/User/_handleWHO.cpp
		src/User/_handlePART.cpp
		src/User/_handleMODE.cpp
		src/User/_handleTOPIC.cpp
		src/Topic.cpp
		include/Topic.hpp
		src/User/_handleLIST.cpp
		src/User/_handleISON.cpp)
