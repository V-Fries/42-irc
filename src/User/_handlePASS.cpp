#include "User.hpp"
#include "NumericReplies.hpp"
#include "ft_Log.hpp"

#include <iostream>
#include <sstream>

void    User::_handlePASS(Server& server, const Command& request) {
    ft::Log::info << "Received PASS request: \"" << request << "\" from user " << _fd
                  << std::endl;
    std::stringstream   response;
    static_cast<void>(server); // TODO remove this
    response << "Received PASS request: " << request << "\n";
    this->_sendMessage(response.str(), server);
//    NumericReplies::Errors::alreadyRegistered(_fd, "Test");
//    if (_isRegistered) {
//
//    }
}
