#include "SignalHandler.hpp"
#include "Server.hpp"
#include "ft_Exception.hpp"

#include <csignal>
#include <cstdlib>

Server* SignalHandler::_server;

void SignalHandler::init(Server& server) {
    _server = &server;

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
    _server->stop(128 + signal);
}
