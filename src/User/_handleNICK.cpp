#include "User.hpp"
#include "ft.hpp"
#include "Server.hpp"

#include <vector>

#include "NumericReplies.hpp"

void    User::_handleNICK(Server& server, const std::vector<ft::String>& args) {
    ft::Log::info << "Received NICK request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::noNicknameGiven(*this, server);
        return;
    }

    ft::String nickname = args[0].substr(0,  User::maxNickNameLength);
    nickname.toLower();
    if (_checkNickname(nickname, server)) {
        _nickName = nickname;
        _registerUserIfReady(server);
    }
}

bool    User::_checkNickname(const ft::String &nickName, const Server &server) {
    if (ft::String("$:#&").find(nickName[0]) != ft::String::npos) {
        NumericReplies::Error::erroneousNick(*this, server, nickName);
        return (false);
    }
    if (nickName.find_first_of(" ,*?!@.") != ft::String::npos) {
        NumericReplies::Error::erroneousNick(*this, server, nickName);
        return (false);
    }
    if (server.nicknameIsTaken(nickName)) {
        NumericReplies::Error::nickInUse(*this, server, nickName);
        return (false);
    }
    return (true);
}
