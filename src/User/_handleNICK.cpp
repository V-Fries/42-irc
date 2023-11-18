#include "User.hpp"
#include "ft.hpp"
#include "Server.hpp"

#include <vector>

#include "NumericReplies.hpp"

void    User::_handleNICK(Server&server, const std::vector<std::string>& args) {
    if (args.empty()) {
        NumericReplies::Error::noNicknameGiven(*this, server);
        return;
    }

    const std::string nickname = ft::String::toLower(args[0].substr(0,  User::maxNickNameLength));
    if (_checkNickname(nickname, server)) {
        _nickName = nickname;
        _registerUserIfReady(server);
    }
}

bool    User::_checkNickname(const std::string &nickName, const Server &server) {
    if (std::string("$:#&").find(nickName[0]) != std::string::npos) {
        NumericReplies::Error::erroneousNick(*this, server, nickName);
        return (false);
    }
    if (nickName.find_first_of(" ,*?!@.") != std::string::npos) {
        NumericReplies::Error::erroneousNick(*this, server, nickName);
        return (false);
    }
    if (server.nicknameIsTaken(nickName)) {
        NumericReplies::Error::nickInUse(*this, server, nickName);
        return (false);
    }
    return (true);
}
