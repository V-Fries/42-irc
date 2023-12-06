#include "NumericReplies.hpp"
#include "User.hpp"

void User::_handleISON(Server& server,
                       const std::vector<ft::String>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "ISON");
        return;
    }

    NumericReplies::Reply::isOn(*this, args, server);
}
