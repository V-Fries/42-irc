#pragma once

#include <string>
#include <sstream>

#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"

#define RPL_WELCOME "001"

class NumericReplies {
    public:
        class HeaderConstructor {
            public:
                HeaderConstructor(const std::string& number,
                                  const std::string& hostname);

                const std::stringstream&    getContent() const;
            private:
                std::stringstream _content;
        };

        class Error {
            public:
                static std::string  alreadyRegistered(const std::string& nickName);

                static std::string  needMoreParameters(const std::string& nickName,
                                                       const std::string& cmdName);
        };

        class Reply {
            public:
        };
};

std::ostream&   operator<<(std::ostream& os,
                           const NumericReplies::HeaderConstructor& headerConstructor);