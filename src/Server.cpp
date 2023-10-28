/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:01:28 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 17:01:45 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "RegisterSocket.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

const char *Server::SocketException::what() const throw() {
    return (_msg);
}

Server::Server(const uint16_t &port, const std::string &password) {
    PollFD              pollFd;
    struct sockaddr_in  addr;

    _socketToAdd = NULL;
    (void) password;
    memset(&pollFd, 0, sizeof(PollFD));
    pollFd.fd = socket(AF_INET, SOCK_STREAM, 0);
    if (pollFd.fd == -1) {
        throw Server::SocketException((char *) "Socket failed");
    }
    addr.sin_port = htons(port);
    addr.sin_family= AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(pollFd.fd, reinterpret_cast<const sockaddr *>(&addr), sizeof (addr)) == -1) {
        throw Server::SocketException((char *) "Bind failed");
    }
    if (listen(pollFd.fd, 10)) {
        throw Server::SocketException((char *) "Listen failed");
    }
    pollFd.events = POLLIN;
    _pollFDs.push_back(pollFd);
    _sockets[pollFd.fd] = new RegisterSocket();
}

void Server::run() {
    bool running = true;
    std::vector<PollFD>::iterator   to_remove;

    while (running) {
        poll(&_pollFDs[0], _pollFDs.size(), 0);
        for (std::vector<PollFD>::iterator it(_pollFDs.begin()); it != _pollFDs.end(); it++) {
            if (it->revents & POLLHUP || it->revents & POLLRDHUP) {
                std::cout << it->fd << " disconnected" << std::endl;
                _disconnections.push(it);
            } else _sockets[(*it).fd]->checkRequest(&*it, this);
            it->revents = 0;
        }
        while (!_disconnections.empty()) {
            to_remove = _disconnections.top();
            close(to_remove->fd);
            _sockets.erase(to_remove->fd);
            _pollFDs.erase(to_remove);
            _disconnections.pop();
        }
        if (_socketToAdd) {
            _pollFDs.push_back(_pollFdToAdd);
            _sockets[_pollFdToAdd.fd] = _socketToAdd;
        }
        _socketToAdd = NULL;
    }
}

void Server::addUser(PollFD pollFd,
                     sockaddr addr,
                     socklen_t socklen) throw(std::exception) {
    _pollFdToAdd = pollFd;
    _socketToAdd = new User(pollFd.fd, addr, socklen);
}