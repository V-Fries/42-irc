#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <stdint.h>

class Server {
    public:
        typedef std::map<int, ISocket*>         SocketMap;
        typedef std::map<std::string, User*>    RegisteredMap;

        Server(uint16_t port, const std::string& password);

        ~Server();

        int                 getEpollFD() const;
        void                addUser(User* user);
        static epoll_event  getBaseUserEpollEvent(int userFD);
        void                removeUser(int userFD);
        bool                nicknameIsTaken(const std::string &nick) const;
        void                registerUser(User *user);
        size_t              getNbOfRegisteredUsers() const;

        void    waitForEvents();
        void    handleEvents();
        void    run();
        void    stop(int exitCode);

    private:
        const std::string   _password;

        const int   _epollFD;
        int         _listenSocketFD;

        epoll_event* _events;
        bool        _shouldUpdateEventsSize;
        int         _numberOfEvents;

        SocketMap       _sockets;
        RegisteredMap   _registeredUsers;

        std::map<std::string, Channel>  _channels;
};
