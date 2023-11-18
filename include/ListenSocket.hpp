#pragma once

#include "ISocket.hpp"

#include <stdint.h>

class ListenSocket : public ISocket {
    public:
        explicit ListenSocket(uint16_t port);

        void    handleEvent(uint32_t epollEvents, Server& server);

        int     getFD() const;

        void    startListening() const;

    private:
        const int   _fd;
};
