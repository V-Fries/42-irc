#include "Bot.hpp"
#include "ft.hpp"
#include "NumericReplies.hpp"



static ft::String  constructMessageToUser(const ft::String& sender,
                                          const ft::String& receiver,
                                          const ft::String& body);

static ft::String  constructMessageToKickUser(const ft::String& sender,
                                              const ft::String& channel,
                                              const ft::String& target,
                                              const ft::String& reason);

void Bot::_handlePRIVMSG(const ft::String& author, const std::vector<ft::String>& args) {
    std::cout << "Received PRIVMSG request: " << args << " from " << author << std::endl;
    ft::String  reply;

    if (*args.begin() == "bot")
        reply = ::constructMessageToUser("bot", author,
                                         "received :" + *(args.end() - 1));
    else {
        if ((args.end() - 1)->find("please ") == 0) {
            reply = ::constructMessageToKickUser("bot",
                                                 *args.begin(),
                                                 (args.end() - 1)->c_str() + 7,
                                                 "");
        }
        if ((args.end() - 1)->find("epitech > 42") != ft::String::npos) {
            reply = ::constructMessageToKickUser("bot", *args.begin(),
                                                  author, "42 is better my boy");
        }
    }
    this->sendMessage(reply);
}

static ft::String  constructMessageToUser(const ft::String& sender,
                                          const ft::String& receiver,
                                          const ft::String& body) {
    std::stringstream   message;

    message << ":" << sender << " PRIVMSG " <<
            receiver << " :" << body << "\r\n";
    return (message.str());
}

static ft::String  constructMessageToKickUser(const ft::String& sender,
                                              const ft::String& channel,
                                              const ft::String& target,
                                              const ft::String& reason) {
    std::stringstream   message;

    (void) sender;
    message << "KICK " << channel << " " << target;
    if (!reason.empty()) {
        message << " :" << reason;
    }
    std::cout << "kick message: " << message.str() << std::endl;
    std::cout << "reason: " << reason << std::endl;
    message << "\r\n";
    return (message.str());
}