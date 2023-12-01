#include "User.hpp"

#include <cstdlib>

#include "Server.hpp"
#include "ft_String.hpp"
#include "Command.hpp"
#include "ft_Log.hpp"
#include "ft_Exception.hpp"
#include "NumericReplies.hpp"

#include <iostream>
#include <sys/socket.h>
#include <sstream>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

User::RequestsHandlersMap   User::_requestsHandlers;
std::string                 User::defaultNickname = "*";

User::User(const int fd):
        _fd(fd),
        _nbOfJoinedLocalChannels(0),
        _nbOfJoinedRegularChannels(0),
        _isRegistered(false),
        _nickName(defaultNickname) {
    ft::Log::debug << "User " << fd << " constructor called" << std::endl;
}

int User::getFD() const {
    return _fd;
}

bool User::hasJoinedTheMaxNbOfRegularChannels() const {
    return this->_nbOfJoinedRegularChannels >= maxNbOfJoinedRegularChannels;
}

bool User::hasJoinedTheMaxNbOfLocalChannels() const {
    return this->_nbOfJoinedLocalChannels >= maxNbOfJoinedLocalChannels;
}

void    User::setIsRegistered(const bool isRegistered) {
    _isRegistered = isRegistered;
}

const std::string&  User::getNickName() const {
    return _nickName;
}

const std::string&  User::getUserName() const {
    return _userName;
}

const std::string& User::getRealName() const {
    return _realName;
}

std::string User::getHostMask() const {
    std::stringstream message;

    message << ':' << _nickName << '!' << _userName << '@' << _realName;
    return message.str();
}

void    User::initRequestsHandlers() {
    ft::Log::debug << "Initializing User::_requestsHandlers" << std::endl;
    _requestsHandlers["PASS"] = &User::_handlePASS;
    _requestsHandlers["USER"] = &User::_handleUSER;
    _requestsHandlers["NICK"] = &User::_handleNICK;
    _requestsHandlers["PRIVMSG"] = &User::_handlePRIVMSG;
    _requestsHandlers["JOIN"] = &User::_handleJOIN;
    _requestsHandlers["PING"] = &User::_handlePING;
    _requestsHandlers["WHO"] = &User::_handleWHO;
    _requestsHandlers["PART"] = &User::_handlePART;
    _requestsHandlers["TOPIC"] = &User::_handleTOPIC;
    _requestsHandlers["MODE"] = &User::_handleMODE;
    _requestsHandlers["LIST"] = &User::_handleLIST;
    _requestsHandlers["ISON"] = &User::_handleISON;
    _requestsHandlers["INVITE"] = &User::_handleINVITE;
}

void    User::handleEvent(const uint32_t epollEvents, Server& server) {
    ft::Log::debug << "User " << _fd << " is handling event " << epollEvents << std::endl;
    if (epollEvents & EPOLLHUP || epollEvents & EPOLLRDHUP) {
        ft::Log::debug << "User " << _fd << " received EPOLLHUP || EPOLLRDHUP" << std::endl;
        server.removeUser(this);
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

void User::sendMessage(const std::string &message, const Server& server) {
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

void    User::_handleEPOLLIN(Server& server) {
    char        rcvBuffer[2049];
    ssize_t     end;

    end = recv(_fd, rcvBuffer, 2048, 0); // TODO should EPOLLET be removed temporally
    // TODO if we failed to read the whole
    // TODO request in one go?
    if (end < 0) {
        std::stringstream   errorMessage;
        errorMessage << "Failed to read from socket " << _fd;
        throw ft::Exception(errorMessage.str(), ft::Log::ERROR);
    }
    const std::string stringBuffer = std::string(rcvBuffer, end);
    _requestBuffer += stringBuffer;
    ft::Log::debug << "buffer: " << stringBuffer << std::endl;
    if (_requestBuffer.find('\r') != std::string::npos ||
        _requestBuffer.find('\n') != std::string::npos) {
        _processRequest(server);
    }
}

void    User::_processRequest(Server& server) {

    std::vector<std::string>    messages = ft::String::split(_requestBuffer,
                                                             "\r\n");
    if (*(_requestBuffer.end() - 1) == '\n') {
        _requestBuffer = "";
    } else {
        _requestBuffer = *(messages.end() - 1);
        messages.pop_back();
    }
    for (std::vector<std::string>::iterator it = messages.begin();
         it != messages.end();
         ++it) {
        _handleRequest(server, *it);
    }
}

void    User::_handleRequest(Server& server, const std::string& request) {
    ft::Log::info << "Processing request from user " << _fd << std::endl;

    const Command cmd(request);
    try {
        const RequestHandler requestHandler = _requestsHandlers.at(cmd.getCommand());
        (this->*requestHandler)(server, cmd.getArgs());
    } catch (std::out_of_range &) {
        ft::Log::warning << "Request " << cmd << " from user " << _fd
                           << " was not recognized" << std::endl;
    }
}

void User::_flushMessages(const Server& server) {
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

    NumericReplies::Reply::welcome(*this, server);
    NumericReplies::Reply::yourHost(*this, server);
    NumericReplies::Reply::create(*this, server);
    NumericReplies::Reply::myInfo(*this, server);
    NumericReplies::Reply::iSupport(*this, server);
    NumericReplies::Reply::localUserClient(*this, server);
    NumericReplies::Reply::localUserChannels(*this, server);
    NumericReplies::Reply::localUserMe(*this, server);
    NumericReplies::Reply::localUsers(*this, server);
    NumericReplies::Reply::globalUsers(*this, server);
    NumericReplies::Reply::messageOfTheDay(*this, server);
}
