#pragma once

#include <string>

#define ERR_ALREADYREGISTERED "462"

class NumericReplies {
    public:
        static std::string  constructReplyHeader(const std::string& replyCode,
                                                 const std::string& clientName);

        class Errors {
            public:
                static void alreadyRegistered(int clientFD, const std::string& client);
        };
};
