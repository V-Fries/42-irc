#include "User.hpp"
#include "NumericReplies.hpp"
#include "ft_Log.hpp"

#include <sstream>

void    User::_handlePASS(Server& server, const std::vector<ft::String>& args) {
    ft::Log::info << "Received PASS request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "PASS");
        return;
    }

    if (_isRegistered) {
        NumericReplies::Error::alreadyRegistered(*this, server);
        return;
    }

    if (args[0] != server.getPassword()) {
        NumericReplies::Error::passwordMissMatch(*this, server);
        this->sendErrorAndDestroyUser("Wrong password", server);
        return;
    }

    _passwordWasGiven = true;
}