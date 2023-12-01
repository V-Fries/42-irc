#include "Channel.hpp"
#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static void sendChannelWelcomeMessages(User& user, const Server& server, const Channel& channel);

void User::_handleJOIN(Server& server, const std::vector<std::string>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "JOIN");
        return;
    }

    std::vector<std::string> channelsNames = ft::String::split(args[0], ",");
    for (std::vector<std::string>::const_iterator it = channelsNames.begin(); it != channelsNames.end(); ++it) {
        const Channel *currChannel = server.getChannelByName(*it);
        if (!currChannel) {
            Channel*    newChannel = new Channel(*it, "", this);

            server.addChannel(newChannel); // TODO try catch here since it will throw if the name is bad
            sendChannelWelcomeMessages(*this, server, *server.getChannelByName(*it));
            _channels.push_back(newChannel);
        } else {
            try {
                server.addUserToChannel(*it, this);
                _channels.push_back(server.getChannelByName(*it));
                sendChannelWelcomeMessages(*this, server, *server.getChannelByName(*it));
            } catch (Channel::IsFull& ) {
                NumericReplies::Error::channelIsFull(*this, server, *it);
            }
        }
    }
}

static void sendChannelWelcomeMessages(User& user, const Server& server, const Channel& channel) {
    std::stringstream   message;

    message << user.getHostMask() << " JOIN " << channel.getName() << "\r\n";
    for (Channel::UserContainer::iterator it = channel.getMembers().begin(); it != channel.getMembers().end(); ++it) {
        (*it)->sendMessage(message.str(), server);
    }
    NumericReplies::Reply::namesReply(user, channel, server);
    NumericReplies::Reply::endOfNames(user, channel, server);
}
