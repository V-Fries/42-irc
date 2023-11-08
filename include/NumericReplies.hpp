#pragma once

#include <string>
#include <sstream>

#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"

class NumericReplies {
    public:
        class HeaderConstructor {
            public:
                HeaderConstructor(const std::string& number, const std::string& hostname);

                const std::stringstream&    getContent() const;
            private:
                std::stringstream _content;
        };
        static std::string  isAlreadyRegistered();

        static std::string  notEnoughParameters(const std::string& cmdName);
};

std::ostream&   operator<<(std::ostream& os, const NumericReplies::HeaderConstructor& headerConstructor);