#include "ListenSocket.hpp"
#include "User.hpp"
#include "Server.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>

ListenSocket::ListenSocket(const uint16_t port, const std::string& password):
    _fd(socket(AF_INET, SOCK_STREAM, 0)),
    _password(password) {
    if (_fd == -1) throw std::exception(); // TODO Define a custom exception

    struct sockaddr_in  address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(_fd, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) != 0) {
        close(_fd);
        throw std::exception(); // TODO Define a custom exception
    }
}

void    ListenSocket::handleEvent(const uint32_t epollEvents, Server& server) {
    static_cast<void>(epollEvents);

    int userFD = accept(_fd, NULL, NULL);
    if (userFD == -1) throw std::exception(); // TODO Define a custom exception

    User*   user = new User(userFD);
    try {
        server.addUser(user);
    } catch (const std::exception& e) {
        close(userFD);
        delete user;
        throw e;
    }
}

int ListenSocket::getFD() const {
    return _fd;
}

void    ListenSocket::startListening() const {
    if (listen(_fd, SOMAXCONN) == 0) {
        return ;
    }

    throw std::exception(); // TODO Define a custom exception
}
