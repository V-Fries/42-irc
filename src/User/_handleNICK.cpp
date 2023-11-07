#include "User.hpp"

#include <iostream>

void    User::_handleNICK(Server& server, const Command& request) {
    static_cast<void>(server); // TODO remove this
    std::cerr << "Received NICK request: " << request << std::endl;
}
