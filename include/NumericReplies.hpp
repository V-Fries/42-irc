#pragma once

#include <string>
#include <sstream>

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATE "003"
#define RPL_MYINFO "004"
#define RPL_ISUPPORT "005"

#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"

class NumericReplies {
    public:
        class Reply {
            public:
                static std::string  welcome(const std::string& nickName);
                static std::string  yourHost(const std::string& nickName);
                static std::string  create(const std::string& nickName);
                static std::string  myInfo(const std::string& nickName);
                static std::string  iSupport(const std::string& nickName);
        };

        class Error {
            public:
                static std::string  alreadyRegistered(const std::string& nickName);

                static std::string  needMoreParameters(const std::string& nickName,
                                                       const std::string& cmdName);
        };

    private:
        static std::string  _constructHeader(const std::string& requestID,
                                             const std::string& hostname);
};
