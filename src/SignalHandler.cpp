#include "SignalHandler.hpp"
#include "Server.hpp"

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
        throw std::exception();// TODO make custom exception
    }
    if (sigaction(SIGTERM, &action, NULL) < 0) {
        throw std::exception();// TODO make custom exception
    }
}

void     SignalHandler::handleSignal(int signal) {
    _server->stop(128 + signal);
}
