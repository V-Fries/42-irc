#include "Channel.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "ft_Log.hpp"

void User::_handleWHO(Server& server, const ft::Vector<ft::String>& args) {
    ft::Log::info << "Received WHO request: " << args << " from user " << _fd
                  << std::endl;

    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "WHO");
        return;
    }
    const Channel* channel = server.getChannelByName(args[0]);
    if (!channel) return;
    NumericReplies::Reply::whoReply(*this, *channel, server);
    NumericReplies::Reply::endOfwhoReply(*this, *channel, server);
}
