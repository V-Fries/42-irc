#include "ft.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

static std::string  constructMessageToUser(const User& sender,
                                           const User& receiver,
                                           const std::string& body);

void User::_handlePRIVMSG(Server&server, const std::vector<std::string>&args) {
    std::vector<std::string>    targets;
    User*                       currTarget;

    if (args.size() < 2) {
        _sendMessage(NumericReplies::Error::needMoreParameters(_nickName, "PRIVMSG"),
                     server);
    }
    targets = ft::String::split(args[0], ",");
    for (std::vector<std::string>::const_iterator it = targets.begin();
         it != targets.end();
         ++it) {
        currTarget = server.getUserByNickname(*it);
        if (currTarget) {
            currTarget->_sendMessage(constructMessageToUser(*this,
                                                            *currTarget,
                                                            args[1]),
                                     server);
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