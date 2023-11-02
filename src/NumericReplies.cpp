#include "NumericReplies.hpp"
#include "Server.hpp"

#include <sys/socket.h>

std::string NumericReplies::constructReplyHeader(const std::string& replyCode,
                                             const std::string& clientName) {
    return std::string(":" "localhost" " ")
                + replyCode + " "
                + clientName + " ";
}

void    NumericReplies::Errors::alreadyRegistered(const int clientFD,
                                                  const std::string& client) {
    const std::string   header = NumericReplies::constructReplyHeader(ERR_ALREADYREGISTERED,
                                                                      client);
    const std::string   response = header + ":You may not reregister\n";

    send(clientFD, response.c_str(), response.size(), 0);
}
