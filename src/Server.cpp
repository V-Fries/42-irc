#include "Server.hpp"
#include "ListenSocket.hpp"

#include <iostream>
#include <unistd.h>

Server::Server(const uint16_t port, const std::string& password):
    _epollFD(epoll_create1(0)),
    _events(NULL),
    _numberOfEvents(0),
    _sockets() {
    if (_epollFD == -1) throw (std::exception()); // TODO Define a custom exception

    ListenSocket* listenSocket = new ListenSocket(port, password);

    EpollEvent event;
    event.events = EPOLLIN;
    event.data.fd = listenSocket->getFD();
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, listenSocket->getFD(), &event) == -1) {
        delete listenSocket;
        throw (std::exception()); // TODO Define a custom exception
    }
    _sockets[listenSocket->getFD()] = listenSocket;
    _events = new EpollEvent[_sockets.size()];
}


Server::~Server() {
    close(_epollFD);
    for (SocketMap::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        delete it->second;
    }
    delete[] _events;
}


void Server::waitForEvents() {
    std::cout << "Waiting for events" << std::endl;
    _numberOfEvents = epoll_wait(_epollFD, _events, _sockets.size(), -1);
    if (_numberOfEvents == -1) {
        throw (std::exception()); // TODO Define a custom exception
    }
}

void    Server::handleEvents() {
    EpollEvent*  eventsEnd = _events + _numberOfEvents;

    std::cout << "Handling events" << std::endl;
    for (EpollEvent* it = _events; it != eventsEnd; ++it) {
        _sockets[it->data.fd]->handleEvent(*it, *this);
    }

    if (_shouldUpdateEventsSize) {
        delete[] _events;
        _events = new EpollEvent[_sockets.size()];
        _shouldUpdateEventsSize = false;
    }
}

void    Server::addUser(User* user) {
    std::cout << "Adding a new user" << std::endl;

    EpollEvent event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = user->getFD();
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, user->getFD(), &event) == -1) {
        delete user;
        throw (std::exception()); // TODO Define a custom exception
    }
    _sockets[user->getFD()] = user;
    _shouldUpdateEventsSize = true;
}
