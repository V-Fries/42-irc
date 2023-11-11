#pragma once

#include <string>
#include <sstream>

#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
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
                static std::string  erroneousNick(const std::string& currNickname,
                                                  const std::string& newNickname);
                static std::string  nickInUse(const std::string& currNickname,
                                                  const std::string& newNickname);
        };

        class Reply {
            public:
        };
};

std::ostream&   operator<<(std::ostream& os,
                           const NumericReplies::HeaderConstructor& headerConstructor);