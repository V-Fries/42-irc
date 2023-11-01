#pragma once

#include "ISocket.hpp"
#include "EpollEvent.hpp"

#include <string>

class Server;

class User : public ISocket {
    public:
        User(int fd,
             const std::string& nickName,
             const std::string& userName);

        int                 getFD() const;
        const std::string&  getNickName() const;
        const std::string&  getUserName() const;

        void    handleEvent(const EpollEvent& epollEvent, Server& server);

    private:
        const int           _fd;
        const std::string   _nickName;
        const std::string   _userName;
};
