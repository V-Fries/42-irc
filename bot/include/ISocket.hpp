#pragma once

#include <stdint.h>

class Server;

class ISocket {
    public:
        virtual ~ISocket() {}

        virtual void    handleEvent(uint32_t epollEvents, Server& server) = 0;
};
