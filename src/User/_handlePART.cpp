#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static void sendChannelPartMessages(User& user, const Server& server, const Channel& channel, const std::string& reason);
static void sendChannelPartMessages(User& user, const Server& server, const Channel& channel);

void User::_handlePART(Server& server, const std::vector<std::string>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "PART");
        return;
    }
    std::vector<std::string> channelsNames = ft::String::split(args[0], ",");
    for (std::vector<std::string>::const_iterator it = channelsNames.begin(); it != channelsNames.end(); ++it) {
        Channel *currentChannel = server.getChannelByName(*it);
        if (!currentChannel) {
            NumericReplies::Error::noSuchChannel(*this, *it, server);
        } else if (!currentChannel->doesMemberExist(_fd)) {
            NumericReplies::Error::notOnChannel(*this, *currentChannel, server);
        } else {
            if (args.size() > 1) sendChannelPartMessages(*this, server, *currentChannel, args[1]);
            else sendChannelPartMessages(*this, server, *currentChannel);
            currentChannel->removeMember(this);
        }
    }
}

static void sendChannelPartMessages(User& user, const Server& server, const Channel& channel, const std::string& reason) {
    std::stringstream message;
    message << ":" << user.getNickName() << " PART " << channel.getName() << " :" << reason << "\r\n";

    for (Channel::UserContainer::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it) {
        (*it)->sendMessage(message.str(), server);
    }
}

static void sendChannelPartMessages(User& user, const Server& server, const Channel& channel) {
    std::stringstream message;

    message << ":" << user.getNickName() << " PART " << channel.getName() << "\r\n";
    for (Channel::UserContainer::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it) {
        (*it)->sendMessage(message.str(), server);
    }
}
