/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegisterSocket.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:21:32 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 14:29:02 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include "RegisterSocket.hpp"
#include "Server.hpp"

RegisterSocket::RegisterSocket() {

}

void RegisterSocket::checkRequest(PollFD *pollFd, Server *server) {
    PollFD      clientPollFd;
    sockaddr    addr;
    socklen_t   socklen;

    memset(&addr, 0, sizeof(sockaddr));
    memset(&clientPollFd, 0, sizeof(PollFD));
    socklen = 0;
    if (pollFd->revents & POLLIN) {
        std::cout << "Registration Request received" << std::endl;
        clientPollFd.fd = accept(pollFd->fd, &addr, &socklen);
        clientPollFd.events = POLLIN | POLLHUP | POLLRDHUP | POLLPRI;
        server->addUser(clientPollFd, addr, socklen);
    }
}
