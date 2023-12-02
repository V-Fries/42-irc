#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <sys/epoll.h>
#include <map>
#include <stdint.h>

#define SERVER_NAME "127.0.0.1"
#define REQUEST_LENGTH_LIMIT 512

class Server {
    public:
        typedef int                       fd_t;
        typedef std::map<fd_t, ISocket*>  SocketMap;

        typedef std::string                 NickName;
        typedef std::map<NickName, User*>   RegisteredMap;

        typedef std::string                     ChannelName;
        typedef std::map<ChannelName, Channel*> ChannelMap;

        Server(uint16_t port, const std::string& password);

        ~Server();

        int                 getEpollFD() const;

        void                addUser(User* user);
        static epoll_event  getBaseUserEpollEvent(int userFD);
        void                removeUser(User* user);
        void                removeUser(const std::string& nickName);
        User*               getUserByNickname(const std::string&) const;
        bool                nicknameIsTaken(const std::string &nick) const;
        void                registerUser(User *user);
        size_t              getNbOfRegisteredUsers() const;
        size_t              getPeakRegisteredUserCount() const;

        size_t              getNbOfChannels() const;
        void                addChannel(Channel *channel);
        void                removeChannel(Channel *channel);
        Channel*            getChannelByName(const std::string& name);
        void                addUserToChannel(const std::string& channel, User *user);
        const ChannelMap&   getChannels() const;
        void                addUserToChannel(const std::string& channel, User& user);
        void                addUserToChannel(Channel& channel, User& user);

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

        ChannelMap  _channels;
};
