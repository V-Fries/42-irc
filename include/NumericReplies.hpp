#pragma once

#include <string>
#include <sstream>

#include "Channel.hpp"
#include "Server.hpp"

#define RPL_WELCOME "001"
#define RPL_YOURHOST "002"
#define RPL_CREATE "003"
#define RPL_MYINFO "004"
#define RPL_ISUPPORT "005"
#define RPL_UMODEIS "221"
#define RPL_LUSERCLIENT "251"
#define RPL_LUSERCHANNELS "254"
#define RPL_LUSERME "255"
#define RPL_LOCALUSERS "265"
#define RPL_GLOBALUSERS "266"
#define RPL_ENDOFWHO "315"
#define RPL_CHANNELMODEIS "324"
#define RPL_WHOREPLY "352"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"
#define RPL_MOTD "372"
#define RPL_MOTDSTART "375"
#define RPL_ENDOFMOTD "376"

#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_NOTONCHANNEL "442"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"
#define ERR_CHANNELISFULL "471"
#define ERR_USERSDONTMATCH "502"

class Channel;
class User;
class Server;

class NumericReplies {
    public:
        class Reply {
            public:
                static void welcome(User& user, const Server& server);
                static void yourHost(User& user, const Server& server);
                static void create(User& user, const Server& server);
                static void myInfo(User& user, const Server& server);
                static void iSupport(User& user, const Server& server);

                static void localUserClient(User& user, const Server& server);
                static void localUserChannels(User& user, const Server& server);
                static void localUserMe(User& user, const Server& server);
                static void localUsers(User& user, const Server& server);
                static void globalUsers(User& user, const Server& server);

                static void messageOfTheDay(User& user, const Server& server);

                static void namesReply(User& user, const Channel& channel, const Server& server);
                static void endOfNames(User& user, const Channel& channel, const Server& server);

                static void whoReply(User& user, const Channel& channel, const Server& server);
                static void endOfwhoReply(User& user, const Channel& channel, const Server& server);

                static void currUserModes(User& user, const Server& server);
                static void channelModeIs(User& user, const Channel& channel, const Server& server);
        };

        class Error {
            public:
                static void alreadyRegistered(User& user, const Server& server);

                static void needMoreParameters(User& user, const Server& server,
                                               const std::string& cmdName);
                static void erroneousNick(User& user, const Server& server,
                                          const std::string& newNickname);
                static void nickInUse(User& user, const Server& server,
                                      const std::string& newNickname);
                static void channelIsFull(User& user, const Server& server,
                                          const std::string& channelName);
                static void noNicknameGiven(User& user, const Server& server);

                static void noRecipient(User& user, const std::string& command, const Server& server);

                static void noTextToSend(User& user, const Server& server);

                static void noSuchChannel(User& user, const std::string& channel, const Server& server);

                static void notOnChannel(User& user, const Channel& channel, const Server& server);

                static void userDontMatchView(User& user, const Server& server);
                static void userDontMatchSet(User& user, const Server& server);

                static void noSuchNickname(User& user, const std::string& nickname, const Server& server);
        };

    private:
        static std::string  _constructHeader(const std::string& requestID,
                                             const std::string& hostname);
};
