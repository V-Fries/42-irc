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

//FIXME When the client send an EOF in the request the connection is lost
void User::checkRequest(PollFD *pollFd, Server *server) {
    char        buffer[2049];
    ssize_t     end;
    std::string msg = std::string("");
    std::string conf = ":irc.example.com 001 chris :Welcome to the ExampleNet Internet Relay Chat Network chris\n";

    (void) server;
    if (pollFd->revents & POLLIN) {
        std::cout << "receive request from client " << pollFd->fd << std::endl;
        do {
            end = recv(pollFd->fd, buffer, 2048, 0);
            end = 2048;
            buffer[end] = 0;
            msg += std::string (buffer, end);
        } while (end && buffer[end - 1] != '\n');
        std::cout << "message: " << msg << std::endl;
        send(pollFd->fd, NULL, 0, MSG_CONFIRM);
        std::cout << "MSG_CONFIRM sent" << std::endl;
    } else if (pollFd->revents & POLLPRI) {
        std::cout << "out of band data at client " << pollFd->fd << std::endl;
    }
}