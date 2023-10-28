/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegisterSocket.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beroux <beroux@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 19:21:26 by beroux            #+#    #+#             */
/*   Updated: 2023/10/28 10:03:34 by beroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ISocket.hpp"

class RegisterSocket: public ISocket {
    public:
        explicit RegisterSocket();

        void    checkRequest(PollFD *pollFd, Server *server);
    protected:
};