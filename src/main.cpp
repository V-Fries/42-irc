#include "Server.hpp"
#include "SignalHandler.hpp"
#include "ft.hpp"

#include <limits>
#include <cerrno>
#include <cstdlib>
#include <iostream>

static uint16_t getPort(const std::string& portStr);

int main(int argc, char** argv) {
    ft::Log::setDebugLevel(ft::Log::DEBUG);
    ft::Log::setFileToWriteTo("logs.txt");

    if (argc != 3) {
        ft::Log::critical << "Wrong number of arguments, expected:\n"
                             "\t./ircserv PORT PASSWORD" << std::endl;
        return 1;
    }

    try {
        Server  server(getPort(argv[1]), argv[2]); // TODO check that the password in valid
        User::initRequestsHandlers();
        SignalHandler::init(server);
        server.run();
    } catch (const ft::Exception& e) {
        e.printError();
        return 2;
    }
}

static uint16_t getPort(const std::string& portStr) {
    if (portStr.empty()) {
        throw ft::Exception("Port argument should not be empty", ft::Log::CRITICAL);
    }

    char*       lastChar;
    const long  port = std::strtol(portStr.c_str(), &lastChar, 10);

    if (!std::isdigit(static_cast<unsigned char>(portStr[0])) || *lastChar != '\0') {
        throw ft::Exception("Unexpected character in port argument: \"" + portStr + '\"',
                            ft::Log::CRITICAL);
    }
    if (errno == ERANGE || port < 0 || port > std::numeric_limits<uint16_t>::max()) {
        throw ft::Exception("Value for port should be an uint16",
                            ft::Log::CRITICAL);
    }

    ft::Log::debug << "Port argument is " << static_cast<uint16_t>(port) << std::endl;
    return static_cast<uint16_t>(port);
}
