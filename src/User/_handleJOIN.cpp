#include "Channel.hpp"
#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static bool didUserReachThisChannelTypeLimit(const User& user, const ft::String& channelName);
static void createNewChannel(User& user,
                             Server& server,
                             const ft::String& newChannelName,
                             const ft::String& password);
static void joinExistingChannel(User& user,
                                Server& server,
                                Channel& channel,
                                const ft::String& password);
static void sendChannelWelcomeMessages(User& user,
                                       const Server& server,
                                       const Channel& channel);

void User::_handleJOIN(Server& server, const std::vector<ft::String>& args) {
    ft::Log::info << "Received JOIN request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "JOIN");
        return;
    }

    std::vector<ft::String> channelsNames = args[0].split(",");
    std::vector<ft::String> channelsPasswords;
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
            createNewChannel(*this, server, channelsNames[i], password);
        } else {
            joinExistingChannel(*this, server, *channel, password);
        }
    }
}

static bool didUserReachThisChannelTypeLimit(const User& user, const ft::String& channelName) {
    return (channelName[0] == '#' && user.hasJoinedTheMaxNbOfRegularChannels())
           || (channelName[0] == '&' && user.hasJoinedTheMaxNbOfLocalChannels());
}

static void createNewChannel(User& user,
                             Server& server,
                             const ft::String& newChannelName,
                             const ft::String& password) {
    try {
        server.addChannel(new Channel(newChannelName, password, user));
        sendChannelWelcomeMessages(user, server, *server.getChannelByName(newChannelName));
    } catch (Channel::IncorrectName&) {
        NumericReplies::Error::badChannelMask(user, server, newChannelName);
    }
}

static void joinExistingChannel(User& user,
                                Server& server,
                                Channel& channel,
                                const ft::String& password) {
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
