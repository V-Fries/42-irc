#include "Bot.hpp"

#include <algorithm>
#include <Command.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

#include "ft_Exception.hpp"
#include "ft_String.hpp"
#include "NumericReplies.hpp"

Bot::Bot(const ft::String& serverIp,
         const uint16_t port,
         const ft::String& password,
         const std::vector< std::pair<ft::String, ft::String> >& kickWords):
                                        _serverPassword(password),
                                        _connected(false),
                                        _registered(false),
                                        _lastIndexOfBufferWithNoDelimiters(0),
                                        _kickWords(kickWords) {
    ft::Log::info << "Construct bot" << std::endl;
    _epollFd = epoll_create1(0);
    if (_epollFd == -1) {
        ft::Log::error << "Failed to create epoll fd: " << strerror(errno) << std::endl;
        return;
    }

    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1) {
        ft::Log::error << "Failed to create socket: " << strerror(errno) << std::endl;
        close(_epollFd);
        _epollFd = -1;
        return;
    }

    struct sockaddr_in addr = {};
    addr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (connect(_sockfd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) == -1) {
        ft::Log::error << "Failed to connect to " << serverIp
                       << ":" << port
                       << " : " << strerror(errno) << std::endl;
        close(_sockfd);
        _sockfd = -1;
        close(_epollFd);
        _epollFd = -1;
        return;
    }

    epoll_event event = {};
    event.events = EPOLL_DEFAULT;
    event.data.fd = _sockfd;
    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _sockfd, &event) == -1) {
        close(_sockfd);
        close(_epollFd);
        return;
    }

    this->initRequestsHandlers();
    _connected = true;
}

void    Bot::initRequestsHandlers() {
    ft::Log::debug << "Initializing Bot::_requestsHandlers" << std::endl;
    _requestsHandlers["PRIVMSG"] = &Bot::_handlePRIVMSG;
    _requestsHandlers["INVITE"] = &Bot::_handleINVITE;
    _requestsHandlers["JOIN"] = &Bot::_handleJOIN;
    _requestsHandlers["ERROR"] = &Bot::_handleERROR;
    _requestsHandlers["PART"] = &Bot::_handlePART;
    _requestsHandlers["KICK"] = &Bot::_handleKICK;
    _requestsHandlers["QUIT"] = &Bot::_handleQUIT;
    _requestsHandlers[RPL_WELCOME] = &Bot::_handleWELCOME;
    _requestsHandlers[RPL_LIST] = &Bot::_handleRPL_LIST;
    _requestsHandlers[RPL_NAMREPLY] = &Bot::_handleRPL_NAMREPLY;
    _requestsHandlers[ERR_NONICKNAMEGIVEN] = &Bot::_handleNICKERRORS;
    _requestsHandlers[ERR_ERRONEUSNICKNAME] = &Bot::_handleNICKERRORS;
    _requestsHandlers[ERR_NICKNAMEINUSE] = &Bot::_handleNICKERRORS;
    _requestsHandlers[ERR_PASSWDMISMATCH] = &Bot::_handleERR_PASSWDMISMATCH;
}

void Bot::initConnection() {
    this->sendMessage("PASS " + _serverPassword + "\r\nNICK " + _nicknames.front() + "\r\nUSER a b c d\r\n");
}

Bot::~Bot() {
    close(_sockfd);
    close(_epollFd);
}

void Bot::run() {
    epoll_event event = {};

    ft::Log::debug << "start running bot" << std::endl;
    this->initConnection();
    while (_connected) {
        epoll_wait(_epollFd, &event, 1, -1);

        ft::Log::info << "receive event" << std::endl;
        if (event.events & EPOLLIN) {
            _handleEPOLLIN();
        }
        if (event.events & EPOLLOUT) {
            _flushMessages();
        }
    }
}

void    Bot::_processRequest() {
    ft::Log::info << "process request" << std::endl;
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
        _handleRequest(*it);
    }
}

void    Bot::_handleEPOLLIN() {
    ft::Log::info << "handle EPOLLIN" << std::endl;
    char        rcvBuffer[2049];

    const ssize_t end = recv(_sockfd, rcvBuffer, 2048, 0);
    if (end < 0) {
        std::stringstream   errorMessage;
        errorMessage << "Failed to read from socket " << _sockfd;
        throw ft::Exception(errorMessage.str(), ft::Log::ERROR);
    }
    const ft::String stringBuffer = ft::String(rcvBuffer, end);
    _requestBuffer += stringBuffer;
    if (_requestBuffer.find("\r\n", _lastIndexOfBufferWithNoDelimiters) != ft::String::npos) {
        _processRequest();
    }
}

void    Bot::_handleRequest(const ft::String& request) {
    const Command cmd(request);
    ft::Log::info << "Processing request " << cmd.getCommand() << std::endl;

    RequestHandler  requestHandler;
    try {
        ft::Log::info << "process: " << cmd.getCommand() << std::endl;
        requestHandler = _requestsHandlers.at(cmd.getCommand().copyToUpper());
    } catch (std::out_of_range&) {
        ft::Log::warning << "Request " << cmd.getCommand() << " was not recognized" << std::endl;
        return;
    }
    (this->*requestHandler)(cmd.getAuthor(), cmd.getArgs());
}

void Bot::sendMessage(const ft::String &message) {
    _messagesBuffer.push(message);

    struct epoll_event  event = {};
    event.data.fd = _sockfd;
    event.events = EPOLL_DEFAULT | EPOLLOUT;
    if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, _sockfd, &event) == -1) {
        ft::Log::error << "Failed to make fd " << _sockfd << " wait for EPOLLOUT" << std::endl;
    } else {
        ft::Log::info << "fd " << _sockfd << " now waits for EPOLLOUT" << std::endl;
    }
}

void Bot::addNickname(const ft::String& nickname) {
    _nicknames.push(nickname);
}

void Bot::addNicknames(const std::vector<ft::String>& nicknames) {
    for (std::vector<ft::String>::const_iterator it = nicknames.begin();
         it != nicknames.end();
         ++it) {
        _nicknames.push(*it);
    }
}

void Bot::_flushMessages() {
    ft::String messages;

    while (!_messagesBuffer.empty()) {
        if (_messagesBuffer.front().find("QUIT") == 0) {
            _connected = false;
        }
        messages += _messagesBuffer.front();
        _messagesBuffer.pop();
    }
    if (send(_sockfd, messages.c_str(), messages.length(), 0) < 0) {
        ft::Log::error << "Failed to send messages to " << _sockfd << std::endl;
        _messagesBuffer.push(messages);
        return;
    }

    epoll_event  event = {};
    event.data.fd = _sockfd;
    event.events = EPOLL_DEFAULT;
    if (epoll_ctl(_epollFd, EPOLL_CTL_MOD, _sockfd, &event) == -1) {
        ft::Log::error << "Failed to make fd " << _sockfd << " stop waiting for EPOLLOUT"
                           << std::endl;
    } else {
        ft::Log::info << "fd " << _sockfd << " stopped waiting for EPOLLOUT" << std::endl;
    }
}

void Bot::_removeSomeone(const ft::String& nickname) {
    if (nickname == _nicknames.front())
        return;

    for (std::map< ft::String, std::vector<ft::String> >::iterator channel = _channels.begin();
         channel != _channels.end();
         ++channel) {
        _removeSomeone(nickname, channel->first);
    }
}

void Bot::_removeSomeone(const ft::String& nickname, const ft::String& channelName) {
    if (nickname == _nicknames.front())
        return;

    std::vector<ft::String>&    channel = _channels.at(channelName);

    channel.erase(std::remove(channel.begin(), channel.end(), nickname), channel.end());
    if (channel.size() == 1) {
        this->sendMessage("PART " + channelName + "\r\nf");
        _channels.erase(channelName);
    }
}
