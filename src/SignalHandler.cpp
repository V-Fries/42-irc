#include "SignalHandler.hpp"
#include "ft_Exception.hpp"

#include <csignal>
#include <cstdlib>

Bot* SignalHandler::_bot;

void SignalHandler::init(Bot& bot) {
    SignalHandler::_bot = &bot;

    struct sigaction    action;

    action.sa_handler = &handleSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0) {
        throw ft::Exception("Failed to set SIGINT handler", ft::Log::WARNING);
    }
    if (sigaction(SIGTERM, &action, NULL) < 0) {
        throw ft::Exception("Failed to set SIGTERM handler", ft::Log::WARNING);
    }
    if (sigaction(SIGQUIT, &action, NULL) < 0) {
        throw ft::Exception("Failed to set SIGQUIT handler", ft::Log::WARNING);
    }
}

void     SignalHandler::handleSignal(const int signal) {
    (void) signal;
    _bot->sendMessage("QUIT: stop sig received\r\n");
    _bot->run();
}
