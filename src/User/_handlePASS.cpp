#include "User.hpp"
#include "NumericReplies.hpp"
#include "ft_Log.hpp"

#include <iostream>
#include <sstream>

void    User::_handlePASS(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received PASS request: " << args << " from user " << _fd
                  << std::endl;
    std::stringstream   response;
    static_cast<void>(server); // TODO remove this
    response << "Received PASS request: " << args << "\n";
    this->_sendMessage(response.str(), server);
//    NumericReplies::Errors::alreadyRegistered(_fd, "Test");
//    if (_isRegistered) {
//
//    }
}
