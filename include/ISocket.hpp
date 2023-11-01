#pragma once

#include "EpollEvent.hpp"

class Server;

class ISocket {
    public:
        virtual ~ISocket() {}

        virtual void    handleEvent(const EpollEvent& epollEvent, Server& server) = 0;
};
