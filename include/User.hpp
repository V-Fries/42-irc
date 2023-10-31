#pragma once

#include "ISocket.hpp"

#include <string>

class User : public ISocket {
    public:
        User(int fd,
             const std::string& nickName,
             const std::string& userName,
             bool isOperator);

        int                 getFD() const;
        const std::string&  getNickName() const;
        const std::string&  getUserName() const;
        bool                isOperator() const;

        void                onRequest();

    private:
        const int           _fd;
        const std::string   _nickName;
        const std::string   _userName;
};
