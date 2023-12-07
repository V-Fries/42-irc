#include "User.hpp"
#include "Channel.hpp"
#include "ft.hpp"

#include <sstream>
#include <vector>

#include "Server.hpp"

void    User::_handleQUIT(Server& server,
                          const std::vector<ft::String>& args) {
    std::stringstream   message;
    std::stringstream   reply;

    ft::Log::info << "Process QUIT from " << _nickName << std::endl;

    reply << _nickName << "["
          << _userName << "@"
          << _realName << "] (\"";
    if (!args.empty())
        reply << args[0];
    reply << "\")\r\n";

    message << this->getHostMask() << " QUIT :Quit:";
    if (!args.empty())
        message << args[0];
    message << "\r\n";

    this->sendMessageToConnections(message.str(), server);

    this->sendErrorAndDestroyUser(reply.str(), server);
}
