#include <limits>
#include <sstream>
#include <cstdlib>

#include "Bot.hpp"
#include "SignalHandler.hpp"
#include "CSVParser.hpp"

static uint16_t    getPort(const ft::String& portStr);

int main(const int argc, const char** argv) {
    ft::Log::setDebugLevel(ft::Log::INFO);
    const ft::String  pathToBinary = argv[0];
    const ft::String  pathToWords = ft::String(pathToBinary.c_str(), pathToBinary.rfind('/') + 1) + PATH_TO_WORDS;
    std::vector< std::pair<ft::String, ft::String> > kickWords;

    try {
        kickWords = CSVParser::parseCSVFile(pathToWords, ft::Log::WARNING);
    } catch (ft::Exception& e) {
        e.printError();
        return 1;
    }

    if (argc != 4) {
        return 1;
    }

    try {
        Bot bot = Bot(argv[1], getPort(argv[2]), argv[3], kickWords);
        SignalHandler::init(bot);
        bot.addNickname("bot");
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
