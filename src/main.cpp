#include "Server.hpp"

#include <iostream>

int main(int argc, char** argv) {
    static_cast<void>(argc); // TODO remove me
    static_cast<void>(argv); // TODO remove me

    Server  server(4242, "password"); // TODO Use args to set the server up

    while (true) {
        try {
            server.waitForEvents();
        } catch (const std::exception& e) {
            // TODO handle exception
        }
        server.handleEvents();
    }
}
