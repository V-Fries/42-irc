#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "EpollEvent.hpp"
#include "Channel.hpp"

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <stdint.h>

class Server {
    public:
        typedef std::map<int, ISocket*> SocketMap;

        Server(uint16_t port, const std::string& password);

        ~Server();

        void    addUser(User* user);
        void    removeUser(int userFD);

        void    waitForEvents();
        void    handleEvents();
        void    run();
        void    stop(int exitCode);

    private:
        const int   _epollFD;
        int         _listenSocketFD;

        EpollEvent* _events;
        bool        _shouldUpdateEventsSize;
        int         _numberOfEvents;

        SocketMap   _sockets;

        std::map<std::string, Channel>  _channels;
};
