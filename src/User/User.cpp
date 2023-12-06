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

User::RequestsHandlersMap   User::_requestsHandlers;
ft::String                 User::defaultNickname = "*";

User::User(const int fd):
        _fd(fd),
        _nbOfJoinedLocalChannels(0),
        _nbOfJoinedRegularChannels(0),
        _isRegistered(false),
        _nickName(defaultNickname),
        _lastIndexOfBufferWithNoDelimiters(0) {
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

const ft::String&  User::getNickName() const {
    return _nickName;
}

const ft::String&  User::getUserName() const {
    return _userName;
}

const ft::String& User::getRealName() const {
    return _realName;
}

ft::String User::getHostMask() const {
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
    _requestsHandlers["KICK"] = &User::_handleKICK;
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

void User::sendMessage(const ft::String &message, const Server& server) {
    if (ft::Log::getDebugLevel() <= ft::Log::INFO) {
        const ft::String   messageToPrint(message.begin(), message.end() - 2);
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
    const ft::String stringBuffer = ft::String(rcvBuffer, end);
    _requestBuffer += stringBuffer;
    ft::Log::debug << "User(" << _fd << ")::_requestBuffer += \"" << stringBuffer
                     << '\"' << std::endl;
    if (_requestBuffer.find("\r\n", _lastIndexOfBufferWithNoDelimiters) != ft::String::npos) {
        _processRequest(server);
    }
}

void    User::_processRequest(Server& server) {

    std::vector<ft::String>    messages = _requestBuffer.split("\r\n");
    if (_requestBuffer.endsWith("\r\n")) {
        _requestBuffer = "";
        _lastIndexOfBufferWithNoDelimiters = 0;
    } else {
        _requestBuffer = *(messages.end() - 1);
        _lastIndexOfBufferWithNoDelimiters = _requestBuffer.length() - 1;
        messages.pop_back();
    }
    for (std::vector<ft::String>::iterator it = messages.begin();
         it != messages.end();
         ++it) {
        _handleRequest(server, *it);
    }
}

void    User::_handleRequest(Server& server, const ft::String& request) {
    const Command cmd(request);
    ft::Log::info << "Processing request " << cmd.getCommand() << " from user "
                    << _fd << std::endl;

    RequestHandler requestHandler;
    try {
        requestHandler = _requestsHandlers.at(cmd.getCommand().copyToUpper());
    } catch (std::out_of_range&) {
        NumericReplies::Error::unknownCommand(*this, server, cmd.getCommand());
        ft::Log::warning << "Request was not recognized" << std::endl;
        return;
    }

    if (this->isRegistered() || _isCommandAllowedWhenNotRegistered(requestHandler)) {
        (this->*requestHandler)(server, cmd.getArgs());
    } else {
        NumericReplies::Error::notRegistered(*this, server);
    }
}

bool    User::_isCommandAllowedWhenNotRegistered(RequestHandler requestHandler) {
    return requestHandler == &User::_handlePASS
            || requestHandler == &User::_handleUSER
            || requestHandler == &User::_handleNICK;
}

void User::_flushMessages(const Server& server) {
    ft::Log::info << "Flushing messages destined to user " << _fd << std::endl;

    ft::String messages;
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
