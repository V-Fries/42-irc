#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static ft::String  constructMessageToUser(const User& sender,
                                           const User& receiver,
                                           const ft::String& body);
static ft::String  constructMessageToChannel(const User& sender,
                                              const Channel& receiver,
                                              const ft::String& body);

void User::_handlePRIVMSG(Server& server, const ft::Vector<ft::String>& args) {
    ft::Log::info << "Received PRIVMSG request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::noRecipient(*this, "PRIVMSG", server);
        return;
    }
    if (args.size() < 2) {
        NumericReplies::Error::noTextToSend(*this, server);
        return;
    }
    ft::Vector<ft::String> targets = args[0].split(",");
    for (ft::Vector<ft::String>::const_iterator it = targets.begin();
         it != targets.end();
         ++it) {
        User* currUserTarget = server.getUserByNickname(*it);
        Channel* currChannelTarget = server.getChannelByName(*it);
        if (currUserTarget) {
            currUserTarget->sendMessage(constructMessageToUser(*this,
                                                           *currUserTarget,
                                                           args[1]),
                                    server);
        } else if (currChannelTarget) {
            ft::Log::info << "send message to channel :" << currChannelTarget->getName() << std::endl;
            currChannelTarget->sendMessage(_fd, constructMessageToChannel(*this, *currChannelTarget, args[1]), server);
        }
    }
}

static ft::String  constructMessageToUser(const User& sender,
                                           const User& receiver,
                                           const ft::String& body) {
    std::stringstream   message;

    message << ":" << sender.getNickName() << " PRIVMSG " <<
            receiver.getNickName() << " :" << body << "\r\n";
    return (message.str());
}

static ft::String  constructMessageToChannel(const User& sender,
                                              const Channel& receiver,
                                              const ft::String& body) {
    std::stringstream   message;

    message << sender.getHostMask() << " PRIVMSG " <<
            receiver.getName() << " :" << body << "\r\n";
    return (message.str());
}
