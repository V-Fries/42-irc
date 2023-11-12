#include "User.hpp"
#include "ft_Log.hpp"
#include "ft_String.hpp"
#include "NumericReplies.hpp"

#include <iostream>

void    User::_handleUSER(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received USER request: \"" << args << "\" from user " << _fd
                  << std::endl;
    if (args.size() < 4) {
        NumericReplies::Error::needMoreParameters(*this, server, "USER");
        return;
    }

    if (_isRegistered) {
        NumericReplies::Error::alreadyRegistered(*this, server);
        return;
    }

    // TODO check behaviour when too many parameters are received
    _userName = '~' + args[0];
    _realName = args[3];
    _registerUserIfReady(server);
}
