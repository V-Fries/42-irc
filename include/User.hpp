#pragma once

#include "ISocket.hpp"

#include <string>

class User : public ISocket {
    public:
        User(const std::string& nickName, const std::string& userName);

        int                 getFD() const;
        const std::string&  getNickName() const;
        const std::string&  getUserName() const;
        bool                isOperator() const;

    private:
        const int           _fd;
        const std::string   _nickName;
        const std::string   _userName;
        const bool          _isOperator;
};
