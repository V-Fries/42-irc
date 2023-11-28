#include "NumericReplies.hpp"
#include "User.hpp"

static void sendModes(User* user, Server& server, const std::string& target);

void User::_handleMODE(Server& server, const std::vector<std::string>& args) {
    if (!args.size()) {
        NumericReplies::Error::needMoreParameters(*this, server, "MODE");
        return;
    }
    if (args.size() == 1) {
        sendModes(this, server, args[0]);
        return;
    }
}

static void sendModes(User* user, Server& server, const std::string& target) {
    if (target[0] == '#' || target[0] == '&') {
        const Channel *channelTarget = server.getChannelByName(target);
        if (!channelTarget) {
            NumericReplies::Error::noSuchChannel(*user, target, server);
            return;
        }
        NumericReplies::Reply::channelModeIs(*user, *channelTarget, server);
        NumericReplies::Reply::creationTime(*user, *channelTarget, server);
        return;
    }
    if (!server.getUserByNickname(target)) {
        NumericReplies::Error::noSuchNick(*user, server, target);
        return;
    }
    if (target != user->getNickName()) {
        NumericReplies::Error::userDontMatchView(*user, server);
        return;
    }
    NumericReplies::Reply::currUserModes(*user, server);
}
