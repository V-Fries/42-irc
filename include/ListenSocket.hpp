#pragma once

#include "ISocket.hpp"

#include <stdint.h>
#include <string>

class ListenSocket : public ISocket {
    public:
        ListenSocket(uint16_t port, const std::string& password);

        ~ListenSocket();

        void    handleEvent(const EpollEvent& epollEvent, Server& server);

        int getFD() const;

    private:
        const int           _fd;
        const std::string   _password;
};
