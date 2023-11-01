#include "ListenSocket.hpp"
#include "User.hpp"
#include "Server.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ListenSocket::ListenSocket(const uint16_t port, const std::string& password):
    _fd(socket(PF_INET, SOCK_STREAM, 0)),
    _password(password) {
    if (_fd == -1) throw (std::exception()); // TODO Define a custom exception

    struct sockaddr_in  address;
    address.sin_family = PF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // TODO remove this
//    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(port);

    if (bind(_fd, reinterpret_cast<struct sockaddr*>(&address), sizeof(address)) != 0) {
        close(_fd);
        throw (std::exception()); // TODO Define a custom exception
    }

    if (listen(_fd, 42) != 0) {
        close(_fd);
        throw (std::exception()); // TODO Define a custom exception
    }
}


ListenSocket::~ListenSocket() {
    close(_fd);
}


void    ListenSocket::handleEvent(const EpollEvent& epollEvent, Server& server) {
    static_cast<void>(epollEvent); /* TODO If this isn't used in any implementation
                                           of onRequest, remove it */

    int userFD = accept(_fd, NULL, NULL);
    if (userFD == -1) throw (std::exception()); // TODO Define a custom exception

    User*   user = new User(userFD, "test", "test"); // TODO get proper name
    server.addUser(user);
}


int ListenSocket::getFD() const {
    return _fd;
}
