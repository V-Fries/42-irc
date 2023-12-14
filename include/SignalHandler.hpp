#pragma once

#include "Bot.hpp"

class SignalHandler {
    public:
        static void init(Bot& bot);

    private:
        static void handleSignal(int signal);

        static Bot*  _bot;
};
