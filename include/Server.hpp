/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:03:47 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 16:04:32 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "Server.hpp"

#include <poll.h>
#include <vector>
#include <map>
#include <stdint.h>
#include <queue>
#include <list>
#include <string>
#include <cstring>
#include <stack>

class Server {
    public:
		class SocketException: public std::exception {
            private:
                char *_msg;
			public:
                SocketException(char *msg): _msg(strdup(msg)) {};
				const char * what() const throw();
		};

        Server(const uint16_t &port, const std::string &password);

        void    run();
        void    addUser(PollFD pollFd, sockaddr addr, socklen_t socklen) throw(std::exception);

    private:
        PollFD                                      _pollFdToAdd;
        ISocket                                     *_socketToAdd;
        std::string                                 _password;
        std::vector<PollFD>                         _pollFDs;
        std::map<int, ISocket*>                     _sockets;
        std::stack<std::vector<PollFD>::iterator>   _disconnections;
};
