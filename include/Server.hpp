#pragma once

#include "ISocket.hpp"
#include "User.hpp"
#include "Channel.hpp"

#include <poll.h>
#include <vector>
#include <map>
#include <stdint.h>

class Server {
    public:
        Server(const uint16_t &port, const std::string& password);

    private:
        void    addUser() throw(std::exception);

        typedef pollfd PollFD;

        std::vector<PollFD>             _pollFDs;
        std::vector<ISocket*>           _sockets;
        //      <channelName, channel>
        std::map<std::string, Channel>  _channels;
};
