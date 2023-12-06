#include "User.hpp"
#include "ft_Log.hpp"
#include "ft_String.hpp"
#include "NumericReplies.hpp"

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

static std::string getRealName(int fd, const std::string& userSetRealName);

void    User::_handleUSER(Server& server, const std::vector<std::string>& args) {
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

static std::string getRealName(const int fd, const std::string& userSetRealName) {
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
