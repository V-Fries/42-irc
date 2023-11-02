#include "User.hpp"
#include "NumericReplies.hpp"

#include <iostream>

void    User::_handlePASS(Server& server, const std::string& request) {
    static_cast<void>(server); // TODO remove this
    std::cerr << "Received PASS request: " << request << std::endl;
    NumericReplies::Errors::alreadyRegistered(_fd, "Test");
//    if (_isRegistered) {
//
//    }
}
