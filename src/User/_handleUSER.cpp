#include "User.hpp"
#include "ft_Log.hpp"
#include "ft_String.hpp"
#include "NumericReplies.hpp"

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

static ft::String getRealName(int fd, const ft::String& userSetRealName);

void    User::_handleUSER(Server& server, const ft::Vector<ft::String>& args) {
    ft::Log::info << "Received USER request: " << args << " from user " << _fd
                  << std::endl;

    if (!_passwordWasGiven) {
        this->sendErrorAndDestroyUser("Password was not given", server);
        return;
    }

    if (args.size() < 4) {
        NumericReplies::Error::needMoreParameters(*this, server, "USER");
        return;
    }

    if (_isRegistered) {
        NumericReplies::Error::alreadyRegistered(*this, server);
        return;
    }

    _userName = '~' + args[0];
    _realName = ::getRealName(_fd, args[3]);
    ft::Log::debug << "username: " << _userName << " hostname: " << _realName
                   << std::endl;
    _registerUserIfReady(server);
}

static ft::String getRealName(const int fd, const ft::String& userSetRealName) {
    struct sockaddr_in  addr = {};
    socklen_t           len = (sizeof addr);
    getsockname(fd, reinterpret_cast<struct sockaddr*>(&addr), &len);

    const struct hostent* host = gethostbyname(inet_ntoa(addr.sin_addr));
    if (host == NULL) {
        ft::Log::warning << "gethostbyname failed with h error number: " << h_errno
                           << std::endl;
        return userSetRealName;
    }
    return host->h_name;
}
