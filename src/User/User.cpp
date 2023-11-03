#include "User.hpp"
#include "Server.hpp"
#include "ft_String.hpp"

#include <iostream>
#include <sys/socket.h>

User::RequestsHandlersMap User::_requestsHandlers;

User::User(const int fd):
    _fd(fd),
    _isRegistered(false) {
}


int User::getFD() const {
    return _fd;
}

const std::string&  User::getNickName() const {
    return _nickName;
}

const std::string&  User::getUserName() const {
    return _userName;
}

void    User::initRequestsHandlers() {
    _requestsHandlers["PASS"] = &User::_handlePASS;
    _requestsHandlers["USER"] = &User::_handleUSER;
    _requestsHandlers["NICK"] = &User::_handleNICK;
}

void    User::handleEvent(uint32_t epollEvents, Server& server) {
    std::cout << "User " << _fd << " is handling event " << epollEvents << std::endl;
    if (epollEvents & EPOLLHUP || epollEvents & EPOLLRDHUP) {
        server.removeUser(_fd);
        return;
    } else if (epollEvents & EPOLLIN) {
        _handleEPOLLIN(server);
    } else {
        std::cerr << "Unrecognized event on user " << _fd << std::endl;
    }
}

void    User::_handleEPOLLIN(Server& server) {
    char        rcvBuffer[2049];
    ssize_t     end;
    std::string msg = std::string("");

    end = recv(_fd, rcvBuffer, 2048, 0);
    if (end < 0) throw std::exception(); // TODO make custom exception
    rcvBuffer[end] = 0;
    _buffer += rcvBuffer;
    if (_buffer.find('\n') != std::string::npos) {
        _processRequest(server);
    }
    send(_fd, NULL, 0, MSG_CONFIRM);
}

void    User::_processRequest(Server& server) {
    std::vector<std::string>    messages = ft::String::split(_buffer, "\n");
    if (*(_buffer.end() - 1) == '\n') {
        _buffer = "";
    } else {
        _buffer = *(messages.end() - 1);
        messages.pop_back();
    }
    for (std::vector<std::string>::iterator it(messages.begin()); it != messages.end(); ++it) {
        _handleRequest(server, *it);
    }
}

void    User::_handleRequest(Server& server, const std::string& request) {
    const std::string   requestType = ft::String::getFirstWord(request, ' ');

    try {
        RequestHandler requestHandler = _requestsHandlers.at(requestType);
        (this->*requestHandler)(server, request);
    } catch (std::out_of_range &er) {
        std::cerr << "Unknown request: " << request << std::endl;
        return;
    }
}

void User::sendMessage(const std::string &message) {
    _messagesBuffer.push(message);
}

void User::flushMessages(uint32_t epollEvents) {
    if (!(epollEvents & EPOLLOUT)) return;
    if ((epollEvents & (EPOLLHUP | EPOLLRDHUP))) return;
    while (!_messagesBuffer.empty()) {
        send(_fd, _messagesBuffer.front().c_str(), _messagesBuffer.front().length(), 0);
        _messagesBuffer.pop();
    }
}
