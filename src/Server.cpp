#include "Server.hpp"
#include "ListenSocket.hpp"
#include "ft_Exception.hpp"

#include <unistd.h>
#include <cstdlib>

Server::Server(const uint16_t port, const std::string& password):
    _password(password),
    _epollFD(epoll_create1(0)),
    _listenSocketFD(-1),
    _events(NULL),
    _numberOfEvents(0),
    _peakRegisteredUserCount(0) {
    ft::Log::debug << "Server constructor called" << std::endl;
    if (_epollFD == -1) {
        throw ft::Exception("epoll failed to be created", ft::Log::CRITICAL);
    }

    ListenSocket* listenSocket;
    try {
        listenSocket = new ListenSocket(port);
    } catch (const ft::Exception& ) {
        close(_epollFD);
        throw;
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
    for (ChannelMap::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete (it->second);
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

void    Server::removeUser(User *user) {
    ft::Log::info << "User " << user->getFD() << " disconnected" << std::endl;
    for (ChannelMap::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        it->second->removeMember(user);
        it->second->removeOperator(user->getFD());
        it->second->removeInvitedUser(user->getFD());
    }
    if (epoll_ctl(_epollFD, EPOLL_CTL_DEL, user->getFD(), NULL) == -1) {
        ft::Log::error << "Failed to remove user " << user->getFD() << " from epoll" << std::endl;
    }
    if (close(user->getFD()) != 0) {
        ft::Log::error << "Failed to close socket " << user->getFD() << std::endl;
    }
    if (user) {
        _registeredUsers.erase(user->getNickName());
    }
    _sockets.erase(user->getFD());
    delete user;
    _shouldUpdateEventsSize = true;
}

bool Server::nicknameIsTaken(const std::string &nick) const {
    return (_registeredUsers.find(nick) != _registeredUsers.end());
}

void Server::registerUser(User* user) {
    user->setIsRegistered(true);
    _registeredUsers[user->getNickName()] = user;
    _peakRegisteredUserCount = std::max(_peakRegisteredUserCount,
                                        _registeredUsers.size());
}

size_t  Server::getNbOfRegisteredUsers() const {
    return _registeredUsers.size();
}

size_t  Server::getPeakRegisteredUserCount() const {
    return _peakRegisteredUserCount;
}

size_t  Server::getNbOfChannels() const {
    return _channels.size();
}

void Server::addChannel(Channel* channel) {
    _channels[channel->getName()] = channel;
}

void Server::removeChannel(Channel* channel) {
    _channels.erase(channel->getName());
    delete channel;
}

Channel* Server::getChannelByName(const std::string& name) {
    try {
        return (_channels.at(name));
    } catch (std::out_of_range& ) {
        return (NULL);
    }
}

void Server::addUserToChannel(const std::string& channel, User* user) {

    _channels[channel]->addMember(user);
}

const std::string& Server::getNicknameByFd(const int fd) const {
    User*   user;

    try {
        user = dynamic_cast<User*>(_sockets.at(fd));
    } catch (std::out_of_range& ) {
        throw;
    }
    if (!user) {
        return (User::defaultNickname);
    }
    return (user->getNickName());
}

const std::map<std::string, Channel*>& Server::getChannels() const {
    return (_channels);
}

User*   Server::getUserByNickname(const std::string& nickname) const {
    try {
        return (_registeredUsers.at(nickname));
    } catch (std::out_of_range& ) {
        return (NULL);
    }
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

    const epoll_event* eventsEnd = _events + _numberOfEvents;
    for (const epoll_event* it = _events; it != eventsEnd; ++it) {
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
