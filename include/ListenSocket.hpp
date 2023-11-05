#pragma once

#include "ISocket.hpp"

#include <stdint.h>
#include <string>

class ListenSocket : public ISocket {
    public:
        ListenSocket(uint16_t port);

        void    handleEvent(uint32_t epollEvents, Server& server);

        int     getFD() const;

        void    startListening() const;

    private:
        const int   _fd;
};
