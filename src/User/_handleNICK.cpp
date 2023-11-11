#include "User.hpp"
#include "ft_Log.hpp"
#include "NumericReplies.hpp"

#include <iostream>

void    User::_handleNICK(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received NICK request: " << args << " from user " << _fd
                    << std::endl;
    if (args.empty()) {
        _sendMessage(NumericReplies::Error::needMoreParameters(_nickName, "NICK"),
                     server); // TODO send ERR_NONICKNAMEGIVEN not ERR_NEEDMOREPARAMS
        return;
    }

    // TODO check for ERR_ERRONEUSNICKNAME and ERR_NICKNAMEINUSE
    _nickName = ft::String::toLower(args[0]);
    _registerUserIfReady(server);
}
