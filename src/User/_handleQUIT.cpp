#include "User.hpp"
#include "Channel.hpp"
#include "ft.hpp"

#include <algorithm>
#include <sstream>
#include <vector>

#include "Server.hpp"

static void addUsers(std::vector<User*>& users, const Channel* channel);

void    User::_handleQUIT(Server& server,
                          const std::vector<std::string>& args) {
    std::vector<User*>  users;
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

    for (std::vector<Channel*>::iterator it = _channels.begin();
         it != _channels.end();
         ++it) {
        addUsers(users, *it);
    }

    for (std::vector<User*>::iterator it = users.begin();
         it != users.end();
         ++it) {
        (*it)->sendMessage(message.str(), server);
    }

    this->sendMessage(reply.str(), server);
    _flushMessages(server);
    server.removeUser(_nickName);
}

static void    addUsers(std::vector<User*>& users, const Channel* channel) {
    for (Channel::UserContainer::const_iterator it = channel->getMembers().begin();
         it != channel->getMembers().end();
         ++it) {
        if (std::find(users.begin(), users.end(), *it) == users.end()) {
            users.push_back(*it);
        }
    }
}
