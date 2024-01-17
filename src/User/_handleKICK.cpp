#include "ft.hpp"
#include "NumericReplies.hpp"
#include "User.hpp"

static void sendChannelKickMessages(const User& user,
                                    const Server& server,
                                    Channel&channel,
                                    const ft::String& kicked,
                                    const ft::String& reason);

void User::_handleKICK(Server& server,
                       const ft::Vector<ft::String>& args) {
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

    ft::Vector<ft::String>    nicknames = args[1].split(",");
    for (ft::Vector<ft::String>::const_iterator it = nicknames.begin();
         it != nicknames.end();
         ++it) {
        if (!channelTarget->isMember(*it)) {
            NumericReplies::Error::userNotInChannel(*this, *it, *channelTarget, server);
        } else {
            if (args.size() > 2) {
                sendChannelKickMessages(*this, server, *channelTarget, *it, args[2]);
            }
            else {
                sendChannelKickMessages(*this, server, *channelTarget, *it, "");
            }
            channelTarget->removeMember(server.getUserByNickname(*it));
            if (channelTarget->getMembers().empty())
                server.removeChannel(channelTarget->getName());
        }
    }
}

static void sendChannelKickMessages(const User& user,
                                    const Server& server,
                                    Channel& channel,
                                    const ft::String& kicked,
                                    const ft::String& reason) {
    std::stringstream message;

    message << user.getHostMask() << " KICK "
            << channel.getName() << " "
            << kicked << " :"
            << reason << "\r\n";
    channel.sendMessage(-1, message.str(), server);
}