#include "Server.hpp"
#include "ListenSocket.hpp"
#include "ft_Exception.hpp"

#include <unistd.h>
#include <cstdlib>
#include <algorithm>

Server::Server(const uint16_t port, const ft::String& password):
    _password(password),
    _epollFD(epoll_create1(0)),
    _listenSocketFD(-1),
    _events(NULL),
    _shouldUpdateEventsSize(true),
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

    epoll_event event = {};
    event.events = EPOLLIN;
    event.data.fd = _listenSocketFD;
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, _listenSocketFD, &event) == -1) {
        delete listenSocket;
        close(_listenSocketFD);
        close(_epollFD);
        throw ft::Exception("Failed to add listenSocket to epoll_ctl", ft::Log::CRITICAL);
    }
}

Server::~Server() {
    ft::Log::debug << "Server destructor called" << std::endl;
    delete _sockets[_listenSocketFD];
    _sockets.erase(_listenSocketFD);
    for (SocketMap::iterator it = _sockets.begin(); it != _sockets.end(); ++it) {
        _closeSocket(it->first, false);
        delete it->second;
    }
    _closeSocket(_listenSocketFD, true);
    for (ChannelMap::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;
    }
    delete[] _events;
    _closeFd(_epollFD);
}

void Server::_closeSocket(int fd, bool isListenSocket) const {
    if (!isListenSocket
            && epoll_ctl(_epollFD, EPOLL_CTL_DEL, fd, NULL) == -1) {
        ft::Log::warning << "Failed to del epoll_ctl for socket " << fd << std::endl;
    }

    _closeFd(fd);
}

void Server::_closeFd(int fd) {
    int i = 0;
    do {
        errno = 0;
        if (close(fd) < 0 && errno != EINTR) {
            ft::Log::error << "Error while closing socket " << fd << std::endl;
        }
        ++i;
    } while (errno == EINTR && i < 10);
}

const ft::String& Server::getPassword() const {
    return _password;
}

int     Server::getEpollFD() const {
    return _epollFD;
}

void    Server::addUser(User& user) {
    ft::Log::debug << "Adding user " << user.getFD() << " to server" << std::endl;

    epoll_event event = getBaseUserEpollEvent(user.getFD());
    if (epoll_ctl(_epollFD, EPOLL_CTL_ADD, user.getFD(), &event) == -1) {
        throw ft::Exception("Failed to add new user to epoll", ft::Log::ERROR);
    }
    _sockets[user.getFD()] = &user;
    _shouldUpdateEventsSize = true;
}

void Server::renameUser(User& user, const ft::String& newNickName) {
    std::stringstream   message;
    message << user.getHostMask() << " NICK :" << newNickName << "\r\n";

    _registeredUsers.erase(user.getNickName());
    user.setNickName(newNickName);
    _registeredUsers[user.getNickName()] = &user;

    user.sendMessage(message.str(), *this);
    user.sendMessageToConnections(message.str(), *this);
}

void Server::removeNickNameOfUserCurrentlyRegistering(const ft::String& nickName) {
    _nickNamesOfUsersCurrentlyRegistering.erase(nickName);
}

void Server::addNickNameOfUserCurrentlyRegistering(const ft::String& nickName) {
    _nickNamesOfUsersCurrentlyRegistering.insert(nickName);
}

epoll_event Server::getBaseUserEpollEvent(const int userFD) {
    epoll_event event = {};
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    event.data.fd = userFD;
    return event;
}

void    Server::_removeUser(User& user) {
    ft::Log::info << "User " << user.getFD() << " disconnected" << std::endl;
    for (ChannelMap::iterator it = _channels.begin(); it != _channels.end(); ++it) { // TODO once user has a list of channels they are a member of we'll be able to use that instead
        it->second->removeMember(&user);
        it->second->removeOperator(user.getFD());
        it->second->removeInvitedUser(user.getFD());
    }
    _closeSocket(user.getFD(), false);
    _registeredUsers.erase(user.getNickName());
    _nickNamesOfUsersCurrentlyRegistering.erase(user.getNickName());
    _sockets.erase(user.getFD());
    delete &user;
    _shouldUpdateEventsSize = true;
}

void Server::addUserToDestroyList(User& user) {
    _usersToDestroy.push(&user);
}

void Server::removeUser(const ft::String& nickName) {
    User*   user = _registeredUsers.at(nickName);

    if (user) {
        ft::Log::debug << "remove: " << nickName << std::endl;
        _removeUser(*user);
    }
}

void Server::_destroyUsersToDestroy() {
    while (!_usersToDestroy.empty()) {
        _removeUser(*_usersToDestroy.front());
        _usersToDestroy.pop();
    }
}

bool Server::nicknameIsTaken(const ft::String &nick) const {
    return _registeredUsers.find(nick) != _registeredUsers.end()
            || _nickNamesOfUsersCurrentlyRegistering.contains(nick);
}

void Server::registerUser(User& user) {
    user.setIsRegistered(true);
    _registeredUsers[user.getNickName()] = &user;
    _nickNamesOfUsersCurrentlyRegistering.erase(user.getNickName());
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

void Server::addChannel(Channel& channel) {
    _channels[channel.getName()] = &channel;
}

void Server::removeChannel(Channel& channel) {
    _channels.erase(channel.getName());
    delete &channel;
}

Channel* Server::getChannelByName(const ft::String& name) {
    try {
        return _channels.at(name);
    } catch (std::out_of_range& ) {
        return NULL;
    }
}

void    Server::addUserToChannel(const ft::String& channel, User& user) {
    _channels[channel]->addMember(&user);
}

void    Server::addUserToChannel(Channel& channel, User& user) {
    channel.addMember(&user);
}

const ft::String& Server::getNicknameByFd(const int fd) const {
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

const Server::ChannelMap& Server::getChannels() const {
    return (_channels);
}

User*   Server::getUserByNickname(const ft::String& nickname) const {
    try {
        return (_registeredUsers.at(nickname));
    } catch (std::out_of_range& ) {
        return (NULL);
    }
}

void Server::_waitForEvents() {
    ft::Log::info << "Waiting for events" << std::endl;

    if (_shouldUpdateEventsSize) {
        delete[] _events;
        _events = new epoll_event[_sockets.size()];
        _shouldUpdateEventsSize = false;
    }

    _numberOfEvents = epoll_wait(_epollFD, _events, _sockets.size(), -1);
    if (_numberOfEvents == -1) {
        throw ft::Exception("Failed to wait for epoll events", ft::Log::ERROR);
    }
}

void    Server::_handleEvents() {
    ft::Log::info << "Handling events" << std::endl;

    const epoll_event* eventsEnd = _events + _numberOfEvents;
    for (const epoll_event* it = _events; it != eventsEnd; ++it) {
        try {
            _sockets[it->data.fd]->handleEvent(it->events, *this);
        } catch (const ft::Exception& e) {
            e.printError();
        }
    }
}

void    Server::run() {
    dynamic_cast<ListenSocket*>(_sockets[_listenSocketFD])->startListening();

    while (true) {
        try {
            _waitForEvents();
        } catch (const ft::Exception& e) {
            e.printError();
            continue;
        }

        try {
            _handleEvents();
        } catch (const ft::Exception& e) {
            e.printError();
        }

        _destroyUsersToDestroy();
    }
}

void    Server::stop(const int exitCode) {
    ft::Log::info << "Stopping server" << std::endl;
    this->~Server();
    std::exit(exitCode);
}
