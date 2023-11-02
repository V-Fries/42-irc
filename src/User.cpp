#include "User.hpp"
#include "Server.hpp"
#include "ft_String.hpp"

#include <iostream>
#include <sys/socket.h>

User::RequestsHandlersMap User::_requestsHandlers;

User::User(const int fd,
           const std::string& nickName,
           const std::string& userName):
    _fd(fd),
    _nickName(nickName),
    _userName(userName)
{}


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
        _redirectRequest(server, *it);
    }
}

void    User::_redirectRequest(Server& server, const std::string& request) {
    const std::string   requestType = ft::String::getFirstWord(request, ' ');

    RequestsHandlersMap::const_iterator    requestHandler = _requestsHandlers.find(requestType);
    if (requestHandler == _requestsHandlers.end()) {
        std::cerr << "Unknown request: " << request << std::endl;
        return;
    }
    (this->*requestHandler->second)(server, request);
}

void    User::_handlePASS(Server& server, const std::string& request) {
    static_cast<void>(server); // TODO remove this
    std::cerr << "Received PASS request: " << request << std::endl;
}

void    User::_handleUSER(Server& server, const std::string& request) {
    static_cast<void>(server); // TODO remove this
    std::cerr << "Received USER request: " << request << std::endl;
}

void    User::_handleNICK(Server& server, const std::string& request) {
    static_cast<void>(server); // TODO remove this
    std::cerr << "Received NICK request: " << request << std::endl;
}
