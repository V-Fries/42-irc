/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ISocket.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:05:33 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 10:03:00 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <poll.h>

typedef pollfd PollFD;
class   Server;

class ISocket {
    public:
        virtual void    checkRequest(PollFD *pollFd, Server *server) = 0;
};
