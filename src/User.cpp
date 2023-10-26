#include "User.hpp"

User::User(const std::string& nickName, const std::string& userName):
    _nickName(nickName),
    _userName(userName)
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

const bool  User::isOperator() const {
    return _isOperator;
}
