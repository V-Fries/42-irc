#include "Server.hpp"
#include "SignalHandler.hpp"
#include "ft.hpp"

#include <iostream>

int main(int argc, char** argv) {
    static_cast<void>(argc); // TODO remove me
    static_cast<void>(argv); // TODO remove me
    ft::Log::setDebugLevel(ft::Log::DEBUG);
    ft::Log::setFileToWriteTo("logs.txt");

    User::initRequestsHandlers();

    try {
        Server  server(3443, "password"); // TODO Use args to set the server up
        SignalHandler::init(server);
        server.run();
    } catch (const ft::Exception& e) {
        e.printError();
    }
}
