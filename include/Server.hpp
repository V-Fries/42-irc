#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <sys/epoll.h>
#include <vector>
#include <map>
#include <stdint.h>

#define SERVER_NAME "127.0.0.1"
#define NETWORK_NAME "42IRC"
#define SERVER_VERSION "0.1"
#define CREATION_DATE "November the 9th of 2023"

#define PATH_TO_MOTD "data/MOTD.txt" // TODO this path only works if IRC binary
                                     // TODO is in the current working directory

class Server {
    public:
        typedef std::map<int, ISocket*>         SocketMap;
        typedef std::map<std::string, User*>    RegisteredMap;

        Server(uint16_t port, const std::string& password);

        ~Server();

        int                 getEpollFD() const;
        void                addUser(User* user);
        static epoll_event  getBaseUserEpollEvent(int userFD);
        void                removeUser(User* user);
        User*               getUserByNickname(const std::string&) const;
        bool                nicknameIsTaken(const std::string &nick) const;
        void                registerUser(User *user);
        size_t              getNbOfRegisteredUsers() const;
        size_t              getPeakRegisteredUserCount() const;
        size_t              getNbOfChannels() const;
        void                addChannel(Channel *channel);
        Channel*            getChannelByName(const std::string& name) const;
        void                addUserToChannel(const std::string& channel, User *user);
        const std::string&  getNicknameByFd(int fd) const;

        void    waitForEvents();
        void    handleEvents();
        void    run();
        void    stop(int exitCode);

    private:
        const std::string   _password;

        const int   _epollFD;
        int         _listenSocketFD;

        epoll_event*    _events;
        bool            _shouldUpdateEventsSize;
        int             _numberOfEvents;

        SocketMap       _sockets;
        RegisteredMap   _registeredUsers;

        size_t          _peakRegisteredUserCount;

        std::map<std::string, Channel*>  _channels;
};
