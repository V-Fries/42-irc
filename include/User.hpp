/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 09:20:53 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 11:35:59 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ISocket.hpp"

#include <string>
#include <sys/socket.h>

class User : public ISocket {
    public:
        explicit User(int fd, sockaddr addr, socklen_t addrLen);

        const std::string&  getNickName() const;
        const std::string&  getUserName() const;

        void                checkRequest(PollFD *pollFd, Server *server);

    private:
        void    _processLastMessages();

        sockaddr    _addr;
        socklen_t   _addrLen;
        std::string _buffer;
        std::string _nickName;
        std::string _userName;
};
