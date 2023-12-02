#include "User.hpp"
#include "Channel.hpp"
#include "ft.hpp"

#include <sstream>
#include <vector>

#include "Server.hpp"

void    User::_handleQUIT(Server& server,
                          const std::vector<std::string>& args) {
    std::set<User*>     users;
    std::stringstream   message;
    std::stringstream   reply;

    ft::Log::info << "Process QUIT from " << _nickName << std::endl;

    reply << "ERROR :Closing Link: "
          << _nickName << "["
          << _userName << "@"
          << _realName << "] (\"";
    if (!args.empty())
        reply << args[0];
    reply << "\")\r\n";

    message << this->getHostMask() << " QUIT :Quit:";
    if (!args.empty())
        message << args[0];
    message << "\r\n";

    for (std::set<User*>::iterator it = users.begin();
         it != users.end();
         ++it) {
        if (users.find(*it) == users.end()) {
            (*it)->sendMessage(message.str(), server);
            users.insert(*it);
        }
    }

    this->sendMessage(reply.str(), server);
    _flushMessages(server);
    server.removeUser(_nickName);
}
