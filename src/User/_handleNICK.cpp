#include "User.hpp"
#include "ft_Log.hpp"

#include <iostream>

void    User::_handleNICK(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received NICK request: " << args << " from user " << _fd
                    << std::endl;
    static_cast<void>(server); // TODO remove this
}
