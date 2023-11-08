#include "User.hpp"
#include "NumericReplies.hpp"
#include "ft_Log.hpp"

#include <iostream>
#include <sstream>

void    User::_handlePASS(Server& server, const Command& request) {
    ft::Log::info << "Received PASS request: " << request << " from user " << _fd
                  << std::endl;
    if (request.getArgs().empty()) _sendMessage(NumericReplies::notEnoughParameters("PASS"), server);
    else if (_isRegistered) _sendMessage(NumericReplies::isAlreadyRegistered(), server);
    else _password = request.getArgs()[0];
}
