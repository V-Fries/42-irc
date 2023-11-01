#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "EpollEvent.hpp"

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <stdint.h>

class Server {
    public:
        typedef std::map<int, ISocket*> SocketMap;

        Server(uint16_t port, const std::string& password);

        ~Server();

        void    waitForEvents();
        void    handleEvents();

        void    addUser(User* user);

    private:
        int _epollFD;

        EpollEvent* _events;
        bool        _shouldUpdateEventsSize;
        int         _numberOfEvents;

        SocketMap   _sockets;
};
