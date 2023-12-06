#include <bitset>

#include "NumericReplies.hpp"
#include "User.hpp"

static void sendModes(User* user, Server& server, const ft::String& target);
static void setModes(User&user,
                     Server& server,
                     const ft::String&target,
                     const ft::String&modeChars);
static void sendModesSummary(User& user,
                             uint8_t save,
                             uint8_t newModes,
                             const Server& server);

void User::_handleMODE(Server& server, const std::vector<ft::String>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "MODE");
        return;
    }
    if (args.size() == 1) {
        sendModes(this, server, args[0]);
        return;
    }
    setModes(*this, server, args[0], args[1]);
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
                     const ft::String&target,
                     const ft::String&modeChars) {
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
    char            sign = '+';
    const uint8_t   save = channelTarget->getModes(MODE_INV | MODE_KEY | MODE_LIM | MODE_TOP);

    for (ft::String::const_iterator it = modeChars.begin();
         it != modeChars.end();
         ++it) {
        if (*it == '+' || *it == '-') {
            sign = *it;
        } else if (Channel::availableMhannelModes.find(*it) == ft::String::npos) {
            NumericReplies::Error::unknowMode(user, *it, server);
        } else {
            channelTarget->setMode(sign, *it, save);
        }
    }
    sendModesSummary(user, save, channelTarget->getModes(MODE_INV | MODE_KEY | MODE_LIM | MODE_TOP), server);
}

void sendModesSummary(User& user,
                      uint8_t save,
                      uint8_t newModes, const Server& server) {
    std::stringstream   reply;

    uint8_t         diff = save ^ newModes;
    std::bitset<8>  saveBits = save;
    std::bitset<8>  newBits = newModes;
    std::bitset<8>  diffBits = diff;

    ft::Log::info << "size: " << sizeof(std::bitset<8>) << std::endl;
    ft::Log::info << "save: " << saveBits << "\nnew:  " << newBits << "\ndiff: " << diffBits << std::endl;
    if (diff & MODE_TOP) {
        if (newModes & MODE_TOP)
            ft::Log::info << "Mode topic added" << std::endl;
        else
            ft::Log::info << "Mode topic removed" << std::endl;
    }
    if (diff & MODE_INV) {
        if (newModes & MODE_INV)
            ft::Log::info << "Mode topic added" << std::endl;
        else
            ft::Log::info << "Mode topic removed" << std::endl;
    }
    if (newModes & MODE_KEY || diff & MODE_KEY) {
        if (newModes & MODE_KEY)
            ft::Log::info << "Mode key changed" << std::endl;
        else if (diff & MODE_KEY)
            ft::Log::info << "Mode key removed" << std::endl;
    }
    if (newModes & MODE_LIM || diff & MODE_LIM) {
        if (newModes & MODE_LIM)
            ft::Log::info << "Mode limite changed" << std::endl;
        else if (diff & MODE_LIM)
            ft::Log::info << "Mode limite removed" << std::endl;
    }
    reply << user.getHostMask() << " MODE ";
    reply << "\r\n";
    user.sendMessage(reply.str(), server);
}
