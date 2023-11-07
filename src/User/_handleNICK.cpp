#include "User.hpp"
#include "ft_Log.hpp"

#include <iostream>

void    User::_handleNICK(Server& server, const Command& request) {
    ft::Log::info << "Received NICK request: \"" << request << "\" from user " << _fd
                    << std::endl;
    static_cast<void>(server); // TODO remove this
}
