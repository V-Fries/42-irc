#include "NumericReplies.hpp"
#include "User.hpp"

void User::_handleTOPIC(Server& server, const std::vector<ft::String>& args) {
    if (args.empty()) {
        NumericReplies::Error::needMoreParameters(*this, server, "TOPIC");
        return;
    }

    Channel *target = server.getChannelByName(args[0]);
    if (!target) {
        NumericReplies::Error::noSuchChannel(*this, args[0], server);
        return;
    }
    if (!target->isMember(_fd)) {
        NumericReplies::Error::notOnChannel(*this, *target, server);
        return;
    }
    if (args.size() == 1) {
        if (target->getTopic().getContent().empty())
            NumericReplies::Reply::noTopic(*this, *target, server);
        else {
            NumericReplies::Reply::topic(*this, *target, server);
            NumericReplies::Reply::topicWhoTime(*this, *target, server);
        }
        return;
    }
    if (target->getModes(MODE_TOP) && !target->isOperator(_fd)) {
        NumericReplies::Error::chanOperPrivNeeded(*this, *target, server);
        return;
    }
    target->setTopic(args[1], _nickName);
    for (Channel::UserContainer::iterator it = target->getMembers().begin();
         it != target->getMembers().end();
         ++it) {
        NumericReplies::Reply::topic(**it, *target, server);
    }
}
