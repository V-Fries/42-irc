#include "User.hpp"
#include "Server.hpp"

#include <iostream>
#include <sys/socket.h>

User::User(const int fd,
           const std::string& nickName,
           const std::string& userName):
    _fd(fd),
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

void    User::handleEvent(const EpollEvent& epollEvent, Server& server) {
    char        buffer[2049];
    ssize_t     end;
    std::string msg;

    std::cout << "receive request from client " << _fd << std::endl;
    do {
        end = recv(_fd, buffer, 2048, 0);
        if (end == -1) throw (std::exception()); // TODO Define a custom exception
        msg += std::string(buffer, end);
    } while (end && buffer[end - 1] != '\n');
    std::cout << "message: " << msg << std::endl;
    send(_fd, NULL, 0, MSG_CONFIRM);
    std::cout << "MSG_CONFIRM sent" << std::endl;

    static_cast<void>(epollEvent); // TODO remove me
    static_cast<void>(server); // TODO remove me
}
