#include <bitset>

#include "NumericReplies.hpp"
#include "User.hpp"

static void sendModes(User* user, Server& server, const ft::String& target);
static void setModes(User& user,
                     Server& server,
                     const ft::String& target,
                     ft::Vector<ft::String> args);
static void sendUserModes(User& user, Server& server, const ft::String& target);
static void sendModesSummary(User& user,
                             uint8_t save,
                             const Server& server,
                             Channel& channel);
static void fillChannelModesSummaries(Channel& channel,
                               std::stringstream& addSummary,
                               std::stringstream& removeSummary,
                               std::stringstream& paramsSummary,
                               uint8_t save);


void User::_handleMODE(Server& server, const ft::Vector<ft::String>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "MODE");
        return;
    }
    if (args.size() == 1) {
        sendModes(this, server, args[0]);
        return;
    }
    setModes(*this, server, args[0], ft::Vector<ft::String>(args.begin() + 1, args.end()));
}

static void sendModes(User* user, Server& server, const ft::String& target) {
    if (target[0] == '#' || target[0] == '&') {
        const Channel *channelTarget = server.getChannelByName(target);
        if (!channelTarget) {
            NumericReplies::Error::noSuchChannel(*user, target, server);
            return;
        }
        NumericReplies::Reply::channelModeIs(*user, *channelTarget, server);
        NumericReplies::Reply::creationTime(*user, *channelTarget, server);
        return;
    }
    sendUserModes(*user, server, target);
}

static void setModes(User& user,
                     Server& server,
                     const ft::String& target,
                     ft::Vector<ft::String> args) {
    if (target[0] != '#' && target[0] != '&') {
        sendUserModes(user, server, target);
        return;
    }

    Channel*        channelTarget = server.getChannelByName(target);

    if (!channelTarget->isOperator(user.getFD())) {
        NumericReplies::Error::channelPrivilegesNeeded(user, server, *channelTarget);
        return;
    }

    char            sign = '+';
    const uint8_t   save = channelTarget->getModes();

    for (ft::Vector<ft::String>::iterator arg = args.begin();
         arg < args.end();
         ++arg) {
        ft::Log::info << "arg: " << *arg << std::endl;
        ft::String  currArg = *arg;

        for (ft::String::const_iterator it = currArg.begin();
             it != currArg.end();
             ++it) {
            if (*it == '+' || *it == '-') {
                sign = *it;
            } else if (Channel::availableMhannelModes.find(*it) == ft::String::npos) {
                NumericReplies::Error::unknowMode(user, *it, server);
            } else {
                channelTarget->setMode(user, sign, *it, arg, args.end(), server);
            }
        }
    }
    sendModesSummary(user, save, server, *channelTarget);
}

static void sendUserModes(User& user, Server& server, const ft::String& target) {
    if (!server.getUserByNickname(target)) {
        NumericReplies::Error::noSuchNick(user, server, target);
        return;
    }
    if (target != user.getNickName()) {
        NumericReplies::Error::userDontMatchView(user, server);
        return;
    }
    NumericReplies::Reply::currUserModes(user, server);
}

void sendModesSummary(User& user,
                      uint8_t save,
                      const Server& server,
                      Channel& channel) {
    std::stringstream   reply;
    std::stringstream   addSummary;
    std::stringstream   removeSummary;
    std::stringstream   paramsSummary;

    fillChannelModesSummaries(channel,
                              addSummary,
                              removeSummary,
                              paramsSummary,
                              save);
    if (channel.getNewOperatorsNumber()) {
        addSummary << ft::String(channel.getNewOperatorsNumber(), 'o');
        paramsSummary << " " << channel.getNewOperators();
    }
    if (channel.getRemovedOperatorsNumber()) {
        removeSummary << ft::String(channel.getRemovedOperatorsNumber(), 'o');
        paramsSummary << " " << channel.getRemovedOperators();
    }
    reply << user.getHostMask() << " MODE "
          << channel.getName();
    if (!addSummary.str().empty())
        reply << " +" << addSummary.str();
    if (!removeSummary.str().empty())
        reply << " -" << removeSummary.str();
    if (!paramsSummary.str().empty())
        reply << paramsSummary.str();
    reply << "\r\n";
    channel.sendMessage(-1, reply.str(), server);
}

static void fillChannelModesSummaries(Channel& channel,
                               std::stringstream& addSummary,
                               std::stringstream& removeSummary,
                               std::stringstream& paramsSummary,
                               const uint8_t save) {
    const uint8_t   newModes = channel.getModes();
    const uint8_t   diff = save ^ newModes;

    ft::Log::info << std::bitset<8>(diff) << std::endl;
    if (diff & MODE_TOPIC_PROTECTED) {
        if (newModes & MODE_TOPIC_PROTECTED)
            addSummary << "t";
        else
            removeSummary << "t";
    }
    if (diff & MODE_INVITE_ONLY) {
        if (newModes & MODE_INVITE_ONLY)
            addSummary << "i";
        else
            removeSummary << "i";
    }
    if (newModes & MODE_PASSWORD || diff & MODE_PASSWORD) {
        if (newModes & MODE_PASSWORD) {
            addSummary << "k";
            paramsSummary << " " << channel.getPassword();
        }
        else if (diff & MODE_PASSWORD) {
            removeSummary << "k";
            paramsSummary << " *";
        }
    }
    if (newModes & MODE_LIMIT || diff & MODE_LIMIT) {
        if (newModes & MODE_LIMIT) {
            addSummary << "l";
            paramsSummary << " " << channel.getUserLimit();
        }
        else if (diff & MODE_LIMIT) {
            removeSummary << "l";
        }
    }
}
