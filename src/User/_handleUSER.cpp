#include "User.hpp"
#include "ft_Log.hpp"
#include "ft_String.hpp"

#include <iostream>

void    User::_handleUSER(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received USER request: \"" << args << "\" from user " << _fd
                  << std::endl;
    if (_isRegistered) {
        // TODO send ERR_ALREADYREGISTERED
        _sendMessage("You are already registered\n", server); // TODO remove me
        return;
    }
    if (args.size() < 4) {
        // TODO send ERR_NEEDMOREPARAMS
        _sendMessage("Not enough arguments\n", server); // TODO remove me
        return ;
    }
    // TODO check behaviour when too many parameters are received
    _userName = '~' + args[0];
    _realName = args[3];
    _sendMessage("Success\n", server); // TODO remove me
    // TODO check if user should be registered
}
