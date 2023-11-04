#include "User.hpp"
#include "NumericReplies.hpp"

#include <iostream>
#include <sstream>

void    User::_handlePASS(Server& server, const std::string& request) {
    std::stringstream   response;

    static_cast<void>(server); // TODO remove this
    std::cerr << "Received PASS request: " << request << std::endl;
    response << "Received PASS request: " << request << "\n";
    this->_sendMessage(response.str(), server);
//    NumericReplies::Errors::alreadyRegistered(_fd, "Test");
//    if (_isRegistered) {
//
//    }
}
