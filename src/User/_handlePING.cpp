#include <cstdlib>
#include <sstream>

#include "ft_Log.hpp"
#include "Server.hpp"
#include "User.hpp"

void User::_handlePING(Server&server, const std::vector<std::string>&args) {
    std::stringstream   reply;

    if (!args.empty()) {
        reply << ":" << SERVER_NAME << " PONG " << SERVER_NAME << " :" << args[0] << "\r\n";
        this->sendMessage(reply.str(), server);
    }
}
