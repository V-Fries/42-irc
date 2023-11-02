#include "User.hpp"

#include <iostream>

void    User::_handleUSER(Server& server, const std::string& request) {
    static_cast<void>(server); // TODO remove this
    std::cerr << "Received USER request: " << request << std::endl;
}
