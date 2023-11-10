#include "User.hpp"
#include "ft.hpp"
#include "Server.hpp"

#include <iostream>
#include <vector>

#include "NumericReplies.hpp"

void    User::_handleNICK(Server& server, const std::vector<std::string>& args) {
    std::string nickname;

    if (args.empty()) {
        _sendMessage(NumericReplies::Error::needMoreParameters(_nickName, "NICK"), server);
        return;
    }
    nickname = args[0].substr(0,  9);
    if (_checkNickname(nickname, server)) {
        _nickName = nickname;
        _registerUserIfReady(server);
    }
}

bool    User::_checkNickname(const std::string &nickName, const Server &server) {
    if (std::string("$:#&").find(nickName[0]) != std::string::npos) {
        _sendMessage(NumericReplies::Error::erroneousNick(_nickName, nickName), server);
        return (false);
    }
    if (nickName.find_first_of(" ,*?!@.") != std::string::npos) {
        _sendMessage(NumericReplies::Error::erroneousNick(_nickName, nickName), server);
        return (false);
    }
    if (server.nicknameIsTaken(nickName)) {
        _sendMessage(NumericReplies::Error::nickInUse(_nickName, nickName), server);
        return (false);
    }
    return (true);
}
