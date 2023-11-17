#include "ListenSocket.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "ft_Exception.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>

ListenSocket::ListenSocket(const uint16_t port):
    _fd(socket(AF_INET, SOCK_STREAM, 0)) {
    int opt;

    ft::Log::debug << "ListenSocket constructor called" << std::endl;
    if (_fd == -1) {
        throw ft::Exception("Failed to create ListenSocket socket", ft::Log::CRITICAL);
    }

    struct sockaddr_in  address = {};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(_fd, reinterpret_cast<const sockaddr*>(&address), sizeof(address)) != 0) {
        close(_fd);
        throw ft::Exception("Failed to bind ListenSocket socket", ft::Log::CRITICAL);
    }
    opt = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof (int))) {
        ft::Log::error << "setsockopt reuse port failed" << std::endl;
    }
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof (int))) {
        ft::Log::error << "setsockopt reuse addr failed" << std::endl;
    }
}

void    ListenSocket::handleEvent(const uint32_t epollEvents, Server& server) {
    static_cast<void>(epollEvents);

    ft::Log::info << "Server received a connection request" << std::endl;
    int userFD = accept(_fd, NULL, NULL);
    if (userFD == -1) {
        ft::Log::error << "Failed to accept connection request" << std::endl;
        return;
    }

    User*   user = new User(userFD);
    try {
        server.addUser(user);
        ft::Log::info << "Successfully added new user" << std::endl;
    } catch (const ft::Exception& e) {
        close(userFD);
        delete user;
        e.printError();
    }
}

int ListenSocket::getFD() const {
    return _fd;
}

void    ListenSocket::startListening() const {
    if (listen(_fd, SOMAXCONN) == 0) {
        ft::Log::info << "Server is now listening for incoming connections" << std::endl;
        return ;
    }

    throw ft::Exception("Failed to start listening for incoming connections",
                        ft::Log::CRITICAL);
}
