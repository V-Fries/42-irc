#include "User.hpp"

User::User(const int fd,
           const std::string& nickName,
           const std::string& userName,
           const bool isOperator):
    _nickName(nickName),
    _userName(userName),
    _fd(fd),
    _isOperator(isOperator)
{}


int User::getFD() const {
    return _fd;
}

const std::string&  User::getNickName() const {
    return _nickName;
}

const std::string&  User::getUserName() const {
    return _userName;
}

bool  User::isOperator() const {
    return _isOperator;
}
