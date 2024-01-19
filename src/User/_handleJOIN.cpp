#include "Channel.hpp"
#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static bool didUserReachThisChannelTypeLimit(const User& user, const ft::String& channelName);
static Channel* createNewChannel(User& user,
                                 Server& server,
                                 const ft::String& newChannelName,
                                 const ft::String& password);
static int joinExistingChannel(User& user,
                               Server& server,
                               Channel& channel,
                               const ft::String& password);
static void sendChannelWelcomeMessages(User& user,
                                       const Server& server,
                                       Channel& channel);

void User::_handleJOIN(Server& server, const ft::Vector<ft::String>& args) {
    ft::Log::info << "Received JOIN request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "JOIN");
        return;
    }

    if (args.size() == 1 && args[0] == "0") {
        this->leaveAllChannels();
    }

    const ft::Vector<ft::String>   channelsNames = args[0].split(",");
    ft::Vector<ft::String>         channelsPasswords;

    if (args.size() > 1)
        channelsPasswords = args[1].split(",");

    for (size_t i = 0; i < channelsNames.size(); ++i) {
        if (didUserReachThisChannelTypeLimit(*this, channelsNames[i])) {
            NumericReplies::Error::tooManyChannels(*this, server, channelsNames[i]);
            continue;
        }

        Channel*            channel = server.getChannelByName(channelsNames[i]);
        const ft::String&  password = i < channelsPasswords.size() ? channelsPasswords[i] : "";
        if (channel == NULL) {
            channel = createNewChannel(*this, server, channelsNames[i], password);
            if (channel == NULL) continue;
            sendChannelWelcomeMessages(*this, server, *channel);
        }
        if (joinExistingChannel(*this, server, *channel, password) == -1) {
            continue;
        }
        _channels[channel->getName()] = channel;
        if (*channel->getName().begin() == '#')
            _nbOfJoinedRegularChannels++;
        else if (*channel->getName().begin() == '&')
            _nbOfJoinedLocalChannels++;
    }
}

static bool didUserReachThisChannelTypeLimit(const User& user, const ft::String& channelName) {
    return (channelName[0] == '#' && user.hasJoinedTheMaxNbOfRegularChannels())
           || (channelName[0] == '&' && user.hasJoinedTheMaxNbOfLocalChannels());
}

static Channel* createNewChannel(User& user,
                                 Server& server,
                                 const ft::String& newChannelName,
                                 const ft::String& password) {
    try {
        Channel*    newChannel = new Channel(newChannelName, password, user);
        server.addChannel(newChannel);
        return newChannel;
    } catch (Channel::IncorrectName&) {
        NumericReplies::Error::badChannelMask(user, server, newChannelName);
    }
    return NULL;
}

static int joinExistingChannel(User&user,
                               Server&server,
                               Channel&channel,
                               const ft::String&password) {
    if (channel.isMember(user.getFD())) {
        return -1;
    }
    if (channel.isInviteOnly() && !channel.wasUserInvited(user.getFD())) {
        NumericReplies::Error::inviteOnlyChannel(user, server, channel);
        return -1;
    }

    if (channel.getModes(MODE_PASSWORD) && password != channel.getPassword()) {
        NumericReplies::Error::badChannelKey(user, server, channel);
        return -1;
    }

    try {
        server.addUserToChannel(channel, user);
        channel.removeInvitedUser(user.getFD());
        sendChannelWelcomeMessages(user, server, channel);
    } catch (Channel::IsFull&) {
        NumericReplies::Error::channelIsFull(user, server, channel.getName());
        return -1;
    }
    return 0;
}

static void sendChannelWelcomeMessages(User& user,
                                       const Server& server,
                                       Channel& channel) {
    std::stringstream   message;

    message << user.getHostMask() << " JOIN " << channel.getName() << "\r\n";

    channel.sendMessage(-1, message.str(), server);
    if (!channel.getTopic().getSetAt().empty()) {
        NumericReplies::Reply::topic(user, channel, server);
        NumericReplies::Reply::topicWhoTime(user, channel, server);
    }
    NumericReplies::Reply::namesReply(user, channel, server);
    NumericReplies::Reply::endOfNames(user, channel, server);
}
