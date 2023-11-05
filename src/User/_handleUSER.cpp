#include "User.hpp"
#include "ft_Log.hpp"

#include <iostream>

void    User::_handleUSER(Server& server, const std::string& request) {
    ft::Log::info << "Received USER request: \"" << request << "\" from user " << _fd
                  << std::endl;
    static_cast<void>(server); // TODO remove this
}
