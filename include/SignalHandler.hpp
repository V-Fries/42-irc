#pragma once

#include "Server.hpp"

class SignalHandler {
    public:
        static void init(Server& server);

    private:
        static void handleSignal(int signal);

        static Server*  _server;
};
