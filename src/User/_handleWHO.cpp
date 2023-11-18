#include "Channel.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

void User::_handleWHO(Server&server, const std::vector<std::string>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "WHO");
        return;
    }
    const Channel* channel = server.getChannelByName(args[0]);
    if (!channel) return;
    NumericReplies::Reply::whoReply(*this, *channel, server);
    NumericReplies::Reply::endOfwhoReply(*this, *channel, server);
}
