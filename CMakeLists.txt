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
		lib/libftpp/src/String/findFirstNotOfString.cpp
		include/SignalHandler.hpp
		src/SignalHandler.cpp
		lib/libftpp/src/String/getFirstWord.cpp
		lib/libftpp/src/String/getFirstWord.cpp
		lib/libftpp/src/String/skipChars.cpp
		src/Command.cpp
		lib/libftpp/include/ft_Set.hpp
		lib/libftpp/include/ft_Set.hpp
		lib/libftpp/include/ft_Exception.hpp
		lib/libftpp/src/Exception.cpp
		lib/libftpp/src/Log.cpp
		lib/libftpp/include/ft_Log.hpp
		lib/libftpp/include/ft_Io.hpp
		lib/libftpp/src/Io.cpp
		lib/libftpp/src/String/toLowerOrUpper.cpp
		lib/libftpp/src/String/map.cpp
		lib/libftpp/src/String/startsOrEndsWith.cpp
		include/Bot.hpp
		src/Bot/Bot.cpp
		src/Bot/_handleINVITE.cpp
		src/Bot/_handlePRIVMSG.cpp
		src/Bot/_handleERR_PASSWDMISMATCH.cpp
		src/Bot/_handleIGNORE.cpp
		src/Bot/_handleERROR.cpp
		src/Bot/_handleNICKERRORS.cpp
		src/Bot/_handleWELCOME.cpp
		src/Bot/_handleRPL_LIST.cpp
		include/CSVParser.hpp
		src/CSVParser.cpp
		src/Bot/_handleJOIN.cpp
		src/Bot/_handleRPL_NAMREPLY.cpp
		src/Bot/_handleQUIT.cpp
		src/Bot/_handleKICK.cpp
		src/Bot/_handlePART.cpp)
