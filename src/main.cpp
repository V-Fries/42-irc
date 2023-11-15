#include "Server.hpp"
#include "SignalHandler.hpp"
#include "ft.hpp"

#include <limits>
#include <sstream>
#include <cerrno>
#include <cstdlib>
#include <iostream>

static uint16_t     getPort(const std::string& portStr);
static std::string  getPassword(const std::string& password);
static uint16_t     getMaxPasswordLength();

int main(int argc, char** argv) {
    ft::Log::setDebugLevel(ft::Log::DEBUG);
    ft::Log::setFileToWriteTo("logs.txt");

    if (argc != 3) {
        ft::Log::critical << "Wrong number of arguments, expected:\n"
                             "\t./ircserv PORT PASSWORD" << std::endl;
        return 1;
    }

    try {
        Server  server(getPort(argv[1]), getPassword(argv[2]));
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

static std::string getPassword(const std::string& password) {
    if (password.empty()) {
        throw ft::Exception("Password argument should not be empty", ft::Log::CRITICAL);
    }
    if (password.size() > getMaxPasswordLength()) {
        std::stringstream   errorMessage;
        errorMessage << "Password argument should be no longer than "
                       << getMaxPasswordLength() << " characters";
        throw ft::Exception(errorMessage.str(), ft::Log::CRITICAL);
    }

    for (std::string::const_iterator it(password.begin()); it != password.end(); ++it) {
        if (std::isspace(static_cast<unsigned char>(*it))) {
            throw ft::Exception("Password argument should not contain any whitespaces",
                                ft::Log::CRITICAL);
        }
    }

    return password;
}

static uint16_t  getMaxPasswordLength() {
    const uint8_t   passRequestLength = 7; // Length of "PASS \r\n"

    return REQUEST_LENGTH_LIMIT - passRequestLength;
}
