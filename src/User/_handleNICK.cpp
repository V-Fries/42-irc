#include "User.hpp"
#include "ft.hpp"
#include "Server.hpp"

#include <vector>

#include "NumericReplies.hpp"

static bool isNickNameValid(User& user,
                            const ft::String &realNickName,
                            const ft::String& fullNickName,
                            const Server &server);

void    User::_handleNICK(Server& server, const ft::Vector<ft::String>& args) {
    ft::Log::info << "Received NICK request: " << args << " from user " << _fd
                  << std::endl;

    if (!_passwordWasGiven) {
        this->sendErrorAndDestroyUser("Password was not given", server);
        return;
    }

    if (args.empty()) {
        NumericReplies::Error::noNicknameGiven(*this, server);
        return;
    }

    ft::String nickname = args[0].substr(0,  User::maxNickNameLength);
    nickname.toLower();
    if (!isNickNameValid(*this, nickname, args[0], server)) {
        return;
    }

    if (_isRegistered) {
        server.renameUser(*this, nickname);
    } else {
        server.removeNickNameOfUserCurrentlyRegistering(_nickName);
        _nickName = nickname;
        server.addNickNameOfUserCurrentlyRegistering(_nickName);
        _registerUserIfReady(server);
    }
}

static bool isNickNameValid(User& user,
                            const ft::String &realNickName,
                            const ft::String& fullNickName,
                            const Server &server) {
    if (ft::String("$:#&").find(realNickName[0]) != ft::String::npos
        || realNickName.find_first_of(" ,*?!@.") != ft::String::npos) {
        NumericReplies::Error::erroneousNick(user, server, fullNickName);
        return false;
    }
    if (server.nicknameIsTaken(realNickName)) {
        NumericReplies::Error::nickInUse(user, server, realNickName);
        return false;
    }
    return true;
}