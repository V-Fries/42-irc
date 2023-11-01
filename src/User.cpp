/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:22:32 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 17:14:27 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include "User.hpp"
#include "Server.hpp"
#include "ft.hpp"

User::User(int fd, sockaddr addr, socklen_t addrLen) {
    (void) fd;
    _addr = addr;
    _addrLen = addrLen;
    _nickName = "";
    _userName = "";
}

const std::string&  User::getNickName() const {
    return _nickName;
}

const std::string&  User::getUserName() const {
    return _userName;
}

void User::checkRequest(PollFD *pollFd, Server *server) {
    char        rcvBuffer[2049];
    ssize_t     end;
    std::string msg = std::string("");

    (void) server;
    if (pollFd->revents & POLLIN) {
        end = recv(pollFd->fd, rcvBuffer, 2048, 0);
        rcvBuffer[end] = 0;
        _buffer += rcvBuffer;
        if (_buffer.find('\n') != std::string::npos) {
            _processLastMessages();
        }
        send(pollFd->fd, NULL, 0, MSG_CONFIRM);
    }
}

void User::_processLastMessages() {
    std::vector<std::string> messages = ft::String::split(_buffer, "\n", 0);
    if (*(_buffer.end() - 1) == '\n') {
        _buffer = "";
    } else {
        _buffer = *(messages.end() - 1);
        messages.pop_back();
    }
    for (std::vector<std::string>::iterator it(messages.begin()); it != messages.end(); it++) {
        std::cout << "message: " << *it << std::endl;
    }
}
