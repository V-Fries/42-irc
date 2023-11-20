#include "User.hpp"
#include "NumericReplies.hpp"
#include "ft_Log.hpp"

#include <sstream>

void    User::_handlePASS(Server& server, const std::vector<std::string>& args) {
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

    _password = args[0];
    _registerUserIfReady(server);
}
