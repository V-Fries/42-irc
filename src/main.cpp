#include <limits>
#include <sstream>
#include <cstdlib>

#include "Bot.hpp"
#include "SignalHandler.hpp"

static uint16_t    getPort(const ft::String& portStr);
static ft::String  getPassword(const ft::String& password);

int main(const int ac, const char** av) {
    // ft::Log::setDebugLevel(ft::Log::INFO);

    if (ac != 3) {
        return (1);
    }

    try {
        Bot bot = Bot(getPort(av[1]), getPassword(av[2]));
        SignalHandler::init(bot);
        bot.run();
    } catch (const ft::Exception& e) {
        e.printError();
        return 2;
    }
}

static uint16_t getPort(const ft::String& portStr) {
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

static ft::String getPassword(const ft::String& password) {
    if (password.empty()) {
        throw ft::Exception("Password argument should not be empty", ft::Log::CRITICAL);
    }

    for (ft::String::const_iterator it(password.begin()); it != password.end(); ++it) {
        if (std::isspace(static_cast<unsigned char>(*it))) {
            throw ft::Exception("Password argument should not contain any whitespaces",
                                ft::Log::CRITICAL);
        }
    }

    return password;
}