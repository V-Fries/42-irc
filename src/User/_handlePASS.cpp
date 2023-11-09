#include "User.hpp"
#include "NumericReplies.hpp"
#include "ft_Log.hpp"

#include <iostream>
#include <sstream>

void    User::_handlePASS(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received PASS request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        _sendMessage(NumericReplies::Error::needMoreParameters(_nickName, "PASS"),
                     server);
        return;
    }

    if (_isRegistered) {
        _sendMessage(NumericReplies::Error::alreadyRegistered(_nickName), server);
        return;
    }

    _password = args[0];
    _registerUserIfReady(server);
}
