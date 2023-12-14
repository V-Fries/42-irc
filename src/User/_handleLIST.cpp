#include "NumericReplies.hpp"
#include "User.hpp"

void User::_handleLIST(Server& server,
                       const ft::Vector<ft::String>& args) {
    (void) args;
    NumericReplies::Reply::listStart(*this, server);
    NumericReplies::Reply::list(*this, server);
    NumericReplies::Reply::listEnd(*this, server);
}