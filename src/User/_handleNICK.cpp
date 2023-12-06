#include "User.hpp"
#include "ft.hpp"
#include "Server.hpp"

#include <vector>

#include "NumericReplies.hpp"

static bool isNickNameValid(User& user,
                          const std::string &realNickName,
                          const std::string& fullNickName,
                          const Server &server);

void    User::_handleNICK(Server& server, const std::vector<std::string>& args) {
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

    const std::string nickname = ft::String::toLower(args[0].substr(0,  User::maxNickNameLength));
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
                            const std::string &realNickName,
                            const std::string& fullNickName,
                            const Server &server) {
    if (std::string("$:#&").find(realNickName[0]) != std::string::npos
        || realNickName.find_first_of(" ,*?!@.") != std::string::npos) {
        NumericReplies::Error::erroneousNick(user, server, fullNickName);
        return false;
    }
    if (server.nicknameIsTaken(realNickName)) {
        NumericReplies::Error::nickInUse(user, server, realNickName);
        return false;
    }
    return true;
}