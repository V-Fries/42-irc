#include "User.hpp"
#include "Server.hpp"
#include "ft_String.hpp"
#include "Command.hpp"
#include "ft_Log.hpp"
#include "ft_Exception.hpp"
#include "NumericReplies.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sstream>

User::RequestsHandlersMap User::_requestsHandlers;

User::User(const int fd):
    _fd(fd),
    _isRegistered(false),
    _nickName("*") {
    ft::Log::debug << "User " << fd << " constructor called" << std::endl;
}

int User::getFD() const {
    return _fd;
}

void    User::setIsRegistered(bool isRegistered) {
    _isRegistered = isRegistered;
}

const std::string&  User::getNickName() const {
    return _nickName;
}

const std::string&  User::getUserName() const {
    return _userName;
}

void    User::initRequestsHandlers() {
    ft::Log::debug << "Initializing User::_requestsHandlers" << std::endl;
    _requestsHandlers["PASS"] = &User::_handlePASS;
    _requestsHandlers["USER"] = &User::_handleUSER;
    _requestsHandlers["NICK"] = &User::_handleNICK;
    _requestsHandlers["PRIVMSG"] = &User::_handlePRIVMSG;
}

void    User::handleEvent(uint32_t epollEvents, Server& server) {
    ft::Log::debug << "User " << _fd << " is handling event " << epollEvents << std::endl;
    if (epollEvents & EPOLLHUP || epollEvents & EPOLLRDHUP) {
        ft::Log::debug << "User " << _fd << " received EPOLLHUP || EPOLLRDHUP" << std::endl;
        server.removeUser(_fd);
        return;
    }
    if (epollEvents & EPOLLIN) {
        ft::Log::debug << "User " << _fd << " received EPOLLIN" << std::endl;
        _handleEPOLLIN(server);
    }
    if (epollEvents & EPOLLOUT) {
        ft::Log::debug << "User " << _fd << " received EPOLLOUT" << std::endl;
        this->_flushMessages(server);
    }
}

bool User::isRegistered() const {
    return (_isRegistered);
}

void    User::_handleEPOLLIN(Server& server) {
    char        rcvBuffer[2049];
    ssize_t     end;
    std::string msg = std::string("");

    end = recv(_fd, rcvBuffer, 2048, 0); // TODO should EPOLLET be removed temporally
                                         // TODO if we failed to read the whole 
                                         // TODO request in one go?
    if (end < 0) {
        std::stringstream   errorMessage;
        errorMessage << "Failed to read from socket " << _fd;
        throw ft::Exception(errorMessage.str(), ft::Log::ERROR);
    }
    rcvBuffer[end] = 0;
    _requestBuffer += rcvBuffer;
    if (_requestBuffer.find('\n') != std::string::npos) {
        _processRequest(server);
    }
    send(_fd, NULL, 0, MSG_CONFIRM);
}

void    User::_processRequest(Server& server) {
    std::vector<std::string>    messages = ft::String::split(_requestBuffer, "\r\n",
                                                             SPLIT_ON_CHARACTER_SET);
    if (*(_requestBuffer.end() - 1) == '\n') {
        _requestBuffer = "";
    } else {
        _requestBuffer = *(messages.end() - 1);
        messages.pop_back();
    }
    for (std::vector<std::string>::iterator it(messages.begin());
         it != messages.end();
         ++it) {
        _handleRequest(server, *it);
    }
}

void    User::_handleRequest(Server& server, const std::string& request) {
    ft::Log::info << "Processing request from user " << _fd << std::endl;

    Command cmd(request);
    try {
        RequestHandler requestHandler = _requestsHandlers.at(cmd.getCommand());
        (this->*requestHandler)(server, cmd.getArgs());
    } catch (std::out_of_range &er) {
        ft::Log::warning << "Request " << cmd << " from user " << _fd
                           << " was not recognized" << std::endl;
        return;
    }
}

void User::_sendMessage(const std::string &message, Server& server) {
    if (ft::Log::getDebugLevel() <= ft::Log::INFO) {
        const std::string   messageToPrint(message.begin(), message.end() - 2);
        ft::Log::info << "Adding message \"" << messageToPrint << "\" to user "
                        << _fd << " _messagesBuffer" << std::endl;
    }

    _messagesBuffer.push(message);

    struct epoll_event  event = Server::getBaseUserEpollEvent(_fd);
    event.events |= EPOLLOUT;
    if (epoll_ctl(server.getEpollFD(), EPOLL_CTL_MOD, _fd, &event) == -1) {
        ft::Log::error << "Failed to make user " << _fd << " wait for EPOLLOUT" << std::endl;
    } else {
        ft::Log::info << "User " << _fd << " now waits for EPOLLOUT" << std::endl;
    }
}

void User::_sendMessage(const std::string &message, const Server& server) {
    if (ft::Log::getDebugLevel() <= ft::Log::INFO) {
        const std::string   messageToPrint(message.begin(), message.end() - 2);
        ft::Log::info << "Adding message \"" << messageToPrint << "\" to user "
                        << _fd << " _messagesBuffer" << std::endl;
    }

    _messagesBuffer.push(message);

    struct epoll_event  event = Server::getBaseUserEpollEvent(_fd);
    event.events |= EPOLLOUT;
    if (epoll_ctl(server.getEpollFD(), EPOLL_CTL_MOD, _fd, &event) == -1) {
        ft::Log::error << "Failed to make user " << _fd << " wait for EPOLLOUT" << std::endl;
    } else {
        ft::Log::info << "User " << _fd << " now waits for EPOLLOUT" << std::endl;
    }
}

void User::_flushMessages(Server& server) {
    ft::Log::info << "Flushing messages destined to user " << _fd << std::endl;

    std::string messages;
    while (!_messagesBuffer.empty()) {
        messages += _messagesBuffer.front();
        _messagesBuffer.pop();
    }
    if (send(_fd, messages.c_str(), messages.length(), 0) < 0) {
        ft::Log::error << "Failed to send messages to " << _fd << std::endl;
        _messagesBuffer.push(messages);
        return;
    }

    epoll_event  event = Server::getBaseUserEpollEvent(_fd);
    if (epoll_ctl(server.getEpollFD(), EPOLL_CTL_MOD, _fd, &event) == -1) {
        ft::Log::error << "Failed to make user " << _fd << " stop waiting for EPOLLOUT"
                           << std::endl;
    } else {
        ft::Log::info << "User " << _fd << " stopped waiting for EPOLLOUT" << std::endl;
    }
}

void    User::_registerUserIfReady(Server& server) {
    if (_password.empty() || _nickName == "*" || _userName.empty()) return;

    server.registerUser(this);

    _sendMessage(NumericReplies::Reply::welcome(_nickName), server);
    _sendMessage(NumericReplies::Reply::yourHost(_nickName), server);
    _sendMessage(NumericReplies::Reply::create(_nickName), server);
    _sendMessage(NumericReplies::Reply::myInfo(_nickName), server);
    _sendMessage(NumericReplies::Reply::iSupport(_nickName), server);
    _sendMessage(NumericReplies::Reply::localUserClient(_nickName,
                                                        server.getNbOfRegisteredUsers()),
                 server);
    _sendMessage(NumericReplies::Reply::localUserChannels(_nickName,
                                                          server.getNbOfChannels()),
                 server);
    _sendMessage(NumericReplies::Reply::localUserMe(_nickName,
                                                    server.getNbOfRegisteredUsers()),
                 server);
    _sendMessage(NumericReplies::Reply::localUsers(_nickName,
                                                   server.getNbOfRegisteredUsers(),
                                                   server.getPeakRegisteredUserCount()),
                 server);
    _sendMessage(NumericReplies::Reply::globalUsers(_nickName,
                                                   server.getNbOfRegisteredUsers(),
                                                   server.getPeakRegisteredUserCount()),
                 server);
    _sendMessage(NumericReplies::Reply::messageOfTheDay(_nickName), server);
}
