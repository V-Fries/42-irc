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

        typedef ft::String                 NickName;
        typedef std::map<NickName, User*>   RegisteredMap;

        typedef ft::String                     ChannelName;
        typedef std::map<ChannelName, Channel*> ChannelMap;

        Server(uint16_t port, const ft::String& password);

        ~Server();

        const ft::String&  getPassword() const;

        int                 getEpollFD() const;

        void                addUser(User& user);

        void    renameUser(User& user, const ft::String& newNickName);

        void    removeNickNameOfUserCurrentlyRegistering(const ft::String& nickName);
        void    addNickNameOfUserCurrentlyRegistering(const ft::String& nickName);

        static epoll_event  getBaseUserEpollEvent(int userFD);
        void                addUserToDestroyList(User& user);
        User*               getUserByNickname(const ft::String&) const;
        bool                nicknameIsTaken(const ft::String &nick) const;
        void                registerUser(User& user);
        size_t              getNbOfRegisteredUsers() const;
        size_t              getPeakRegisteredUserCount() const;

        size_t              getNbOfChannels() const;
        void                addChannel(Channel& channel);
        void                removeChannel(Channel& channel);
        Channel*            getChannelByName(const ft::String& name);
        const ChannelMap&   getChannels() const;
        void                addUserToChannel(const ft::String& channel, User& user);
        void                addUserToChannel(Channel& channel, User& user);

        const ft::String&  getNicknameByFd(int fd) const;

        void    run();
        void    stop(int exitCode);

    private:
        void        _closeSocket(int fd, bool isListenSocket) const;
        static void _closeFd(int fd);

        void    _removeUser(User& user);
        void    _destroyUsersToDestroy();

        void    _waitForEvents();
        void    _handleEvents();

        const ft::String   _password;

        const int   _epollFD;
        int         _listenSocketFD;

        epoll_event*    _events;
        bool            _shouldUpdateEventsSize;
        int             _numberOfEvents;

        SocketMap       _sockets;
        RegisteredMap   _registeredUsers;

        ft::Set<std::string>   _nickNamesOfUsersCurrentlyRegistering;

        std::queue<User*>   _usersToDestroy;

        size_t          _peakRegisteredUserCount;

        ChannelMap  _channels;
};
