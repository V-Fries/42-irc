#include <bitset>

#include "NumericReplies.hpp"
#include "User.hpp"

static void sendModes(User* user, Server& server, const ft::String& target);
static void setModes(User& user,
                     Server& server,
                     const ft::String& target,
                     std::vector<ft::String> args);
static void sendModesSummary(User& user,
                             uint8_t save,
                             uint8_t newModes,
                             const Server& server,
                             Channel& channel);

void User::_handleMODE(Server& server, const std::vector<ft::String>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "MODE");
        return;
    }
    if (args.size() == 1) {
        sendModes(this, server, args[0]);
        return;
    }
    setModes(*this, server, args[0], std::vector<ft::String>(args.begin() + 1, args.end()));
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
    if (!server.getUserByNickname(target)) {
        NumericReplies::Error::noSuchNick(*user, server, target);
        return;
    }
    if (target != user->getNickName()) {
        NumericReplies::Error::userDontMatchView(*user, server);
        return;
    }
    NumericReplies::Reply::currUserModes(*user, server);
}

static void setModes(User& user,
                     Server& server,
                     const ft::String& target,
                     std::vector<ft::String> args) {
    if (target[0] != '#' && target[0] != '&') {
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

    Channel*        channelTarget = server.getChannelByName(target);

    if (!channelTarget->isOperator(user.getFD())) {
        NumericReplies::Error::channelPrivilegesNeeded(user, server, *channelTarget);
        return;
    }

    char            sign = '+';
    const uint8_t   save = channelTarget->getModes(MODE_INV | MODE_KEY | MODE_LIM | MODE_TOP);

    for (std::vector<ft::String>::iterator arg = args.begin();
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
    sendModesSummary(user, save, channelTarget->getModes(MODE_INV | MODE_KEY | MODE_LIM | MODE_TOP), server, *channelTarget);
}

void sendModesSummary(User& user,
                      uint8_t save,
                      uint8_t newModes,
                      const Server& server,
                      Channel& channel) {
    std::stringstream   reply;
    std::stringstream   addSummary;
    std::stringstream   removeSummary;
    std::stringstream   paramsSummary;
    uint8_t             diff = save ^ newModes;

    if (diff & MODE_TOP) {
        if (newModes & MODE_TOP)
            addSummary << "t";
        else
            removeSummary << "t";
    }
    if (diff & MODE_INV) {
        if (newModes & MODE_INV)
            addSummary << "i";
        else
            removeSummary << "i";
    }
    if (newModes & MODE_KEY || diff & MODE_KEY) {
        if (newModes & MODE_KEY) {
            addSummary << "k";
            paramsSummary << channel.getPassword() << " ";
        }
        else if (diff & MODE_KEY) {
            removeSummary << "k";
            paramsSummary << "* ";
        }
    }
    if (newModes & MODE_LIM || diff & MODE_LIM) {
        if (newModes & MODE_LIM) {
            addSummary << "l";
            paramsSummary << channel.getUserLimit() << " ";
        }
        else if (diff & MODE_LIM) {
            removeSummary << "l";
        }
    }
    reply << user.getHostMask() << " MODE "
          << channel.getName() << " ";
    if (!addSummary.str().empty())
        reply << "+" << addSummary.str();
    if (!removeSummary.str().empty())
        reply << "-" << removeSummary.str();
    if (!paramsSummary.str().empty())
        reply << " " << paramsSummary.str();
    reply << " " << channel.getNewOperators()
          << " " << channel.getRemovedOperators();
    reply << "\r\n";
    channel.sendMessage(-1, reply.str(), server);
}
