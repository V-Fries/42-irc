#include "User.hpp"
#include "Server.hpp"
#include "NumericReplies.hpp"

static bool userCanInviteAUserToThisChannel(User& user,
                                            const Channel& channel,
                                            const Server& server);

static bool userCanBeInvitedToThisChannel(User& inviter,
                                          const User* invitee,
                                          const std::string& inviteeNickName,
                                          const Server& server,
                                          const Channel& channel);

static void inviteUserToChannel(User& inviter,
                                User& invitee,
                                const Server& server,
                                Channel& channel);

static void sendInvitationToInvitee(const User& inviter,
                                    User& invitee,
                                    const Server& server,
                                    const Channel& channel);

void User::_handleINVITE(Server& server, const std::vector<std::string>& args) {
    if (args.size() < 2) {
        NumericReplies::Error::needMoreParameters(*this, server, "INVITE");
        return;
    }

    Channel* channel(server.getChannelByName(args[1]));
    if (channel == NULL) {
        NumericReplies::Error::noSuchChannel(*this, args[1], server);
        return;
    }

    if (!userCanInviteAUserToThisChannel(*this, *channel, server)) return;

    User* invitedUser = server.getUserByNickname(args[0]);
    if (!userCanBeInvitedToThisChannel(*this, invitedUser, args[0], server, *channel)) {
        return;
    }

    inviteUserToChannel(*this, *invitedUser, server, *channel);
}

static bool userCanInviteAUserToThisChannel(User& user,
                                            const Channel& channel,
                                            const Server& server) {
    if (!channel.isMember(&user)) {
        NumericReplies::Error::notOnChannel(user, channel, server);
        return false;
    }

    if (channel.isInviteOnly() && !channel.isOperator(user.getFD())) {
        NumericReplies::Error::channelPrivilegesNeeded(user, server, channel);
        return false;
    }

    return true;
}

static bool userCanBeInvitedToThisChannel(User& inviter,
                                          const User* invitee,
                                          const std::string& inviteeNickName,
                                          const Server& server,
                                          const Channel& channel) {
    if (invitee == NULL) {
        NumericReplies::Error::noSuchNick(inviter, server, inviteeNickName);
        return false;
    }

    if (channel.isMember(invitee)) {
        NumericReplies::Error::userOnChannel(inviter, server, invitee->getNickName(), channel);
        return false;
    }

    return true;
}

static void inviteUserToChannel(User& inviter,
                                User& invitee,
                                const Server& server,
                                Channel& channel) {
    channel.addInvitedUser(invitee.getFD());

    NumericReplies::Reply::inviting(inviter, server, invitee.getNickName(), channel);
    sendInvitationToInvitee(inviter, invitee, server, channel);
}

static void sendInvitationToInvitee(const User& inviter,
                                    User& invitee,
                                    const Server& server,
                                    const Channel& channel) {
    std::stringstream message;
    message << inviter.getHostMask() << " INVITE " << invitee.getNickName() << ' '
            << channel.getName() << "\r\n";
    invitee.sendMessage(message.str(), server);
}
