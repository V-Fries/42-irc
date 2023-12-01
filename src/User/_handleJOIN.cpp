#include "Channel.hpp"
#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static bool didUserReachThisChannelTypeLimit(const User& user, const std::string& channelName);
static Channel* createNewChannel(User& user,
                                 Server& server,
                                 const std::string& newChannelName,
                                 const std::string& password);
static void joinExistingChannel(User& user,
                                Server& server,
                                Channel& channel,
                                const std::string& password);
static void sendChannelWelcomeMessages(User& user,
                                       const Server& server,
                                       const Channel& channel);

void User::_handleJOIN(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received JOIN request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "JOIN");
        return;
    }

    std::vector<std::string>            channelsNames = ft::String::split(args[0], ",");
    std::vector<std::string>            channelsPasswords;

    if (args.size() > 1)
        channelsPasswords = ft::String::split(args[1], ",");

    for (size_t i = 0; i < channelsNames.size(); ++i) {
        if (didUserReachThisChannelTypeLimit(*this, channelsNames[i])) {
            NumericReplies::Error::tooManyChannels(*this, server, channelsNames[i]);
            continue;
        }

        Channel*            channel = server.getChannelByName(channelsNames[i]);
        const std::string&  password = i < channelsPasswords.size() ? channelsPasswords[i] : "";
        if (!channel) {
            channel = createNewChannel(*this, server, channelsNames[i], password);
            if (!channel) continue;
        }
        joinExistingChannel(*this, server, *channel, password);
        _channels.push_back(channel);
    }
}

static bool didUserReachThisChannelTypeLimit(const User& user, const std::string& channelName) {
    return (channelName[0] == '#' && user.hasJoinedTheMaxNbOfRegularChannels())
           || (channelName[0] == '&' && user.hasJoinedTheMaxNbOfLocalChannels());
}

static Channel* createNewChannel(User& user,
                                 Server& server,
                                 const std::string& newChannelName,
                                 const std::string& password) {
    try {
        Channel*    newChannel = new Channel(newChannelName, password, user);
        server.addChannel(newChannel);
        return newChannel;
    } catch (Channel::IncorrectName&) {
        NumericReplies::Error::badChannelMask(user, server, newChannelName);
    }
    return NULL;
}

static void joinExistingChannel(User& user,
                                Server& server,
                                Channel& channel,
                                const std::string& password) {
    if (channel.isMember(user.getFD())) {
        return; // TODO check if we should send a numeric
    }
    if (channel.isInviteOnly() && !channel.wasUserInvited(user.getFD())) {
        NumericReplies::Error::inviteOnlyChannel(user, server, channel);
        return;
    }

    if (password != channel.getPassword()) {
        NumericReplies::Error::badChannelKey(user, server, channel);
        return;
    }

    try {
        server.addUserToChannel(channel, user);
        channel.removeInvitedUser(user.getFD());
        sendChannelWelcomeMessages(user, server, channel);
    } catch (Channel::IsFull&) {
        NumericReplies::Error::channelIsFull(user, server, channel.getName());
    }
}

static void sendChannelWelcomeMessages(User& user,
                                       const Server& server,
                                       const Channel& channel) {
    std::stringstream   message;

    message << user.getHostMask() << " JOIN " << channel.getName() << "\r\n";
    for (Channel::UserContainer::iterator it = channel.getMembers().begin();
         it != channel.getMembers().end();
         ++it) {
        (*it)->sendMessage(message.str(), server);
    }
    NumericReplies::Reply::topic(user, channel, server);
    NumericReplies::Reply::topicWhoTime(user, channel, server);
    NumericReplies::Reply::namesReply(user, channel, server);
    NumericReplies::Reply::endOfNames(user, channel, server);
}
