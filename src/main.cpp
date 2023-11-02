#include "Server.hpp"
#include "SignalHandler.hpp"

#include <iostream>

int main(int argc, char** argv) {
    static_cast<void>(argc); // TODO remove me
    static_cast<void>(argv); // TODO remove me

    User::initRequestsHandlers();

    try {
        Server  server(4242, "password"); // TODO Use args to set the server up
        SignalHandler::init(server);
        server.run();
    } catch (const std::exception& e) {
        // TODO handle exception
    }
}
