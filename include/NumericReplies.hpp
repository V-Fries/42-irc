#pragma once

#include <string>
#include <sstream>

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATE "003"
#define RPL_MYINFO "004"
#define RPL_ISUPPORT "005"
#define RPL_LUSERCLIENT "251"
#define RPL_LUSERCHANNELS "254"

#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
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
                static std::string  localUserClient(const std::string& nickName,
                                                    size_t nbOfUsers);
                static std::string  localUserChannels(const std::string& nickName,
                                                      size_t nbOfChannels);
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

    private:
        static std::string  _constructHeader(const std::string& requestID,
                                             const std::string& hostname);
};
