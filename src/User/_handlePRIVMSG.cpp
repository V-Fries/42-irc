#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static std::string  constructMessageToUser(const User& sender,
                                           const User& receiver,
                                           const std::string& body);
static std::string  constructMessageToChannel(const User& sender,
                                              const Channel& receiver,
                                              const std::string& body);

void User::_handlePRIVMSG(Server& server, const std::vector<std::string>& args) {
    ft::Log::info << "Received PRIVMSG request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::noRecipient(*this, "PRIVMSG", server);
    }
    if (args.size() < 2) {
        NumericReplies::Error::noTextToSend(*this, server);
    }
    std::vector<std::string> targets = ft::String::split(args[0], ",");
    for (std::vector<std::string>::const_iterator it = targets.begin();
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

static std::string  constructMessageToUser(const User& sender,
                                           const User& receiver,
                                           const std::string& body) {
    std::stringstream   message;

    message << ":" << sender.getNickName() << " PRIVMSG " <<
            receiver.getNickName() << " :" << body << "\r\n";
    return (message.str());
}

static std::string  constructMessageToChannel(const User& sender,
                                              const Channel& receiver,
                                              const std::string& body) {
    std::stringstream   message;

    message << ":" << sender.getNickName() << " PRIVMSG " <<
            receiver.getName() << " :" << body << "\r\n";
    return (message.str());
}
