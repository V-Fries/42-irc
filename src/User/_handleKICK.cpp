#include "ft.hpp"
#include "NumericReplies.hpp"
#include "User.hpp"

static void sendChannelKickMessages(const User& user,
                                    const Server& server,
                                    Channel&channel,
                                    const std::string& kicked,
                                    const std::string& reason);

void User::_handleKICK(Server& server,
                       const std::vector<std::string>& args) {
    if (args.size() < 2) {
        NumericReplies::Error::needMoreParameters(*this, server, "KICK");
        return;
    }
    Channel *channelTarget = server.getChannelByName(args[0]);
    if (!channelTarget) {
        NumericReplies::Error::noSuchChannel(*this, args[0], server);
        return;
    }
    if (!channelTarget->isMember(_fd)) {
        NumericReplies::Error::notOnChannel(*this, *channelTarget, server);
        return;
    }
    if (!channelTarget->isOperator(_fd)) {
        NumericReplies::Error::chanOperPrivNeeded(*this, *channelTarget, server);
        return;
    }

    std::vector<std::string>    nicknames = ft::String::split(args[1], ",");
    for (std::vector<std::string>::const_iterator it = nicknames.begin();
         it != nicknames.end();
         ++it) {
        if (!channelTarget->isMember(*it)) {
            NumericReplies::Error::userNotInChannel(*this, *it, *channelTarget, server);
        } else {
            sendChannelKickMessages(*this, server, *channelTarget, *it, args[2]);
            channelTarget->removeMember(server.getUserByNickname(*it));
        }
    }
}

static void sendChannelKickMessages(const User& user,
                                    const Server& server,
                                    Channel& channel,
                                    const std::string& kicked,
                                    const std::string& reason) {
    std::stringstream message;

    message << user.getHostMask() << " KICK "
            << channel.getName() << " "
            << kicked << " :"
            << reason << "\r\n";
    channel.sendMessage(-1, message.str(), server);
}