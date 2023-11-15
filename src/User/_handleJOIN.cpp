#include "Channel.hpp"
#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static void sendChannelWelcomeMessages(User& user, const Server& server, const Channel& channel);

void User::_handleJOIN(Server&server, const std::vector<std::string>&args) {
    Channel *currChannel;
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "JOIN");
        return;
    }

    std::vector<std::string> channelsNames = ft::String::split(args[0], ",");
    for (std::vector<std::string>::const_iterator it = channelsNames.begin(); it != channelsNames.end(); ++it) {
        currChannel = server.getChannelByName(*it);
        if (!currChannel) {
            server.addChannel(new Channel(*it, "", this));
            sendChannelWelcomeMessages(*this, server, *server.getChannelByName(*it));
        } else {
            try {
                server.addUserToChannel(*it, this);
                sendChannelWelcomeMessages(*this, server, *server.getChannelByName(*it));
            } catch (Channel::IsFull& ) {
                NumericReplies::Error::channelIsFull(*this, server, *it);
            }
        }
    }
}

static void sendChannelWelcomeMessages(User& user, const Server& server, const Channel& channel) {
    std::stringstream   message;

    message << ":" << user.getNickName() << " JOIN " << channel.getName() << "\r\n"; // TODO: send to all channel members
    user.sendMessage(message.str(), server);
    NumericReplies::Reply::namesReply(user, channel, server);
    NumericReplies::Reply::endOfNames(user, channel, server);
}
