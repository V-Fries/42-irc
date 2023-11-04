#include "Server.hpp"
#include "ListenSocket.hpp"
#include "ft_Log.hpp"

#include <iostream>
#include <unistd.h>
#include <cstdlib>

Server::Server(const uint16_t port, const std::string& password):
    _epollFD(epoll_create1(0)),
    _listenSocketFD(),
    _events(NULL),
    _numberOfEvents(0),
    _sockets() {
    if (_epollFD == -1) throw std::exception(); // TODO Define a custom exception

    ListenSocket* listenSocket;
    try {
        listenSocket = new ListenSocket(port, password);
    } catch (const std::exception& e) {
        close(_epollFD);
        throw e;
    }
    _listenSocketFD = listenSocket->getFD();

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = _listenSocketFD;
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, _listenSocketFD, &event) == -1) {
        delete listenSocket;
        close(_epollFD);
        throw std::exception(); // TODO Define a custom exception
    }
    _sockets[_listenSocketFD] = listenSocket;
    _events = new epoll_event[_sockets.size()];
}


Server::~Server() {
    delete _sockets[_listenSocketFD];
    _sockets.erase(_listenSocketFD);
    for (SocketMap::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        epoll_ctl(_epollFD, EPOLL_CTL_DEL, it->first, NULL);
        if (close(it->first) != 0) {
            std::cerr << "Error while closing socket " << it->first << std::endl;
        }
        delete it->second;
    }
    epoll_ctl(_epollFD, EPOLL_CTL_DEL, _listenSocketFD, NULL);
    if (close(_listenSocketFD) != 0) {
        std::cerr << "Error while closing socket " << _listenSocketFD << std::endl;
    }
    delete[] _events;
    _sockets.clear();
    if (close(_epollFD) != 0) {
        std::cerr << "Error while closing socket " << _epollFD << std::endl;
    }
}


int     Server::getEpollFD() const {
    return _epollFD;
}

void    Server::addUser(User* user) {
    std::cout << "Adding a new user" << std::endl;

    epoll_event event = getBaseUserEpollEvent(user->getFD());
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, user->getFD(), &event) == -1) {
        throw std::exception(); // TODO Define a custom exception
    }
    _sockets[user->getFD()] = user;
    _shouldUpdateEventsSize = true;
}

epoll_event Server::getBaseUserEpollEvent(const int userFD) {
    epoll_event event = {};
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    event.data.fd = userFD;
    return event;
}

void    Server::removeUser(const int userFD) {
    std::cerr << "User " << userFD << " disconnected" << std::endl;
    epoll_ctl(_epollFD, EPOLL_CTL_DEL, userFD, NULL);
    if (close(userFD) != 0) {
        std::cerr << "Error while closing socket " << userFD << std::endl;
    }
    delete _sockets[userFD];
    _sockets.erase(userFD);
    _shouldUpdateEventsSize = true;
}


void Server::waitForEvents() {
    std::cout << "Waiting for events" << std::endl;
    _numberOfEvents = epoll_wait(_epollFD, _events, _sockets.size(), -1);
    if (_numberOfEvents == -1) {
        throw std::exception(); // TODO Define a custom exception
    }
}

void    Server::handleEvents() {
    ft::Log::info << "Handling events" << std::endl;

    epoll_event* eventsEnd = _events + _numberOfEvents;
    for (epoll_event* it = _events; it != eventsEnd; ++it) {
        _sockets[it->data.fd]->handleEvent(it->events, *this);
    }

    if (_shouldUpdateEventsSize) {
        delete[] _events;
        _events = new epoll_event[_sockets.size()];
        _shouldUpdateEventsSize = false;
    }
}

void    Server::run() {
    dynamic_cast<ListenSocket*>(_sockets[_listenSocketFD])->startListening();

    while (true) {
        try {
            this->waitForEvents();
        } catch (const std::exception& e) {
            // TODO handle exception
        }

        try {
            this->handleEvents();
        } catch (const std::exception& e) {
            // TODO handle exception
        }
    }
}

void    Server::stop(const int exitCode) {
    std::cerr << "Stopping server" << std::endl;
    this->~Server();
    std::exit(exitCode);
}
