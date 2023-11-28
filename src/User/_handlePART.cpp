#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static void processCurrentChannel(const std::string& channelName,
                                  User* user,
                                  Server& server);
static void processCurrentChannel(const std::string& channelName,
                                  User* user,
                                  Server& server,
                                  const std::string& reason);

static void sendChannelPartMessages(const User& user,
                                    const Server& server,
                                    const Channel& channel,
                                    const std::string& reason);
static void sendChannelPartMessages(const User& user,
                                    const Server& server,
                                    const Channel& channel);

void User::_handlePART(Server& server, const std::vector<std::string>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "PART");
        return;
    }

    std::vector<std::string> channelsNames = ft::String::split(args[0], ",");
    for (std::vector<std::string>::const_iterator it = channelsNames.begin(); it != channelsNames.end(); ++it) {
        if (args.size() > 1)
            processCurrentChannel(*it, this, server, args[1]);
        else
            processCurrentChannel(*it, this, server);
    }
}

void processCurrentChannel(const std::string& channelName,
                           User* user,
                           Server& server) {
    Channel *channel = server.getChannelByName(channelName);

    if (!channel) {
        NumericReplies::Error::noSuchChannel(*user, channelName, server);
        return;
    }
    if (!channel->isMember(user->getFD())) {
        NumericReplies::Error::notOnChannel(*user, *channel, server);
        return;
    }
    sendChannelPartMessages(*user, server, *channel);
    channel->removeMember(user);
    if (channel->getMembers().empty()) {
        server.removeChannel(channel);
    }
}

void processCurrentChannel(const std::string& channelName,
                           User* user,
                           Server& server,
                           const std::string& reason) {
    Channel *channel = server.getChannelByName(channelName);

    if (!channel) {
        NumericReplies::Error::noSuchChannel(*user, channelName, server);
        return;
    }
    if (!channel->isMember(user->getFD())) {
        NumericReplies::Error::notOnChannel(*user, *channel, server);
        return;
    }
    sendChannelPartMessages(*user, server, *channel, reason);
    channel->removeMember(user);
    if (channel->getMembers().empty()) {
        server.removeChannel(channel);
    }
}

static void sendChannelPartMessages(const User& user,
                                    const Server& server,
                                    const Channel& channel,
                                    const std::string& reason) {
    std::stringstream message;
    message << ":" << user.getNickName() << " PART " << channel.getName() << " :" << reason << "\r\n";

    for (Channel::UserContainer::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it) {
        (*it)->sendMessage(message.str(), server);
    }
}

static void sendChannelPartMessages(const User& user,
                                    const Server& server,
                                    const Channel& channel) {
    std::stringstream message;

    message << user.getHostMask() << " PART "
            << channel.getName() << "\r\n";
    for (Channel::UserContainer::iterator it = channel.getMembers().begin();
         it != channel.getMembers().end();
         ++it) {
        (*it)->sendMessage(message.str(), server);
    }
}
