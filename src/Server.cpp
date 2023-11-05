#include "Server.hpp"
#include "ListenSocket.hpp"
#include "ft_Exception.hpp"

#include <iostream>
#include <unistd.h>
#include <cstdlib>

Server::Server(const uint16_t port, const std::string& password):
    _password(password),
    _epollFD(epoll_create1(0)),
    _listenSocketFD(-1),
    _events(NULL),
    _numberOfEvents(0),
    _sockets() {
    ft::Log::debug << "Server constructor called" << std::endl;
    if (_epollFD == -1) {
        throw ft::Exception("epoll failed to be created", ft::Log::CRITICAL);
    }

    ListenSocket* listenSocket;
    try {
        listenSocket = new ListenSocket(port);
    } catch (const ft::Exception& e) {
        close(_epollFD);
        throw e;
    }
    _listenSocketFD = listenSocket->getFD();
    _sockets[_listenSocketFD] = listenSocket;

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = _listenSocketFD;
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, _listenSocketFD, &event) == -1) {
        delete listenSocket;
        close(_listenSocketFD);
        close(_epollFD);
        throw ft::Exception("Failed to add listenSocket to epoll_ctl", ft::Log::CRITICAL);
    }
    _events = new epoll_event[_sockets.size()];
}


Server::~Server() {
    ft::Log::debug << "Server destructor called" << std::endl;
    for (SocketMap::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        epoll_ctl(_epollFD, EPOLL_CTL_DEL, it->first, NULL);
        delete it->second;
        do {
            errno = 0;
            if (close(it->first) < 0 && errno != EINTR) {
                ft::Log::error << "Error while closing socket " << it->first << std::endl;
            }
        } while (errno == EINTR);
    }
    delete[] _events;
    _sockets.clear();
    do {
        errno = 0;
        if (close(_epollFD) < 0 && errno != EINTR) {
            ft::Log::error << "Error while closing epoll " << _epollFD << std::endl;
        }
    } while (errno == EINTR);
}


int     Server::getEpollFD() const {
    return _epollFD;
}

void    Server::addUser(User* user) {
    ft::Log::debug << "Adding user " << user->getFD() << " to server" << std::endl;

    epoll_event event = getBaseUserEpollEvent(user->getFD());
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, user->getFD(), &event) == -1) {
        throw ft::Exception("Failed to add new user to epoll", ft::Log::ERROR);
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
    ft::Log::info << "User " << userFD << " disconnected" << std::endl;
    if (epoll_ctl(_epollFD, EPOLL_CTL_DEL, userFD, NULL) == -1) {
        ft::Log::error << "Failed to remove user " << userFD << " from epoll" << std::endl;
    }
    if (close(userFD) != 0) {
        ft::Log::error << "Failed to close socket " << userFD << std::endl;
    }
    delete _sockets[userFD];
    _sockets.erase(userFD);
    _shouldUpdateEventsSize = true;
}


void Server::waitForEvents() {
    ft::Log::info << "Waiting for events" << std::endl;
    _numberOfEvents = epoll_wait(_epollFD, _events, _sockets.size(), -1);
    if (_numberOfEvents == -1) {
        throw ft::Exception("Failed to wait for epoll events", ft::Log::ERROR);
    }
}

void    Server::handleEvents() {
    ft::Log::info << "Handling events" << std::endl;

    epoll_event* eventsEnd = _events + _numberOfEvents;
    for (epoll_event* it = _events; it != eventsEnd; ++it) {
        try {
            _sockets[it->data.fd]->handleEvent(it->events, *this);
        } catch (const ft::Exception& e) {
            e.printError();
        }
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
        } catch (const ft::Exception& e) {
            e.printError();
            continue;
        }

        try {
            this->handleEvents();
        } catch (const ft::Exception& e) {
            e.printError();
        }
    }
}

void    Server::stop(const int exitCode) {
    ft::Log::info << "Stopping server" << std::endl;
    this->~Server();
    std::exit(exitCode);
}
