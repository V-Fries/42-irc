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
#define RPL_ISON "303"
#define RPL_ENDOFWHO "315"
#define RPL_LISTSTART "321"
#define RPL_LIST "322"
#define RPL_LISTEND "323"
#define RPL_CREATIONTIME "329"
#define RPL_NOTOPIC "331"
#define RPL_TOPIC "332"
#define RPL_TOPICWHOTIME "333"
#define RPL_CHANNELMODEIS "324"
#define RPL_INVITING "341"
#define RPL_WHOREPLY "352"
#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"
#define RPL_MOTD "372"
#define RPL_MOTDSTART "375"
#define RPL_ENDOFMOTD "376"

#define ERR_NOSUCHNICK "401"
#define ERR_NOSUCHCHANNEL "403"
#define ERR_TOOMANYCHANNELS "405"
#define ERR_NORECIPIENT "411"
#define ERR_NOTEXTTOSEND "412"
#define ERR_UNKNOWNCOMMAND "421"
#define ERR_NONICKNAMEGIVEN "431"
#define ERR_ERRONEUSNICKNAME "432"
#define ERR_NICKNAMEINUSE "433"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define ERR_USERONCHANNEL "443"
#define ERR_NOTREGISTERED "451"
#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"
#define ERR_PASSWDMISMATCH "464"
#define ERR_CHANNELISFULL "471"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANNELKEY "475"
#define ERR_BADCHANMASK "476"
#define ERR_CHANOPRIVSNEEDED "482"
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

                static void namesReply(User& user,
                                       const Channel& channel,
                                       const Server& server);
                static void endOfNames(User& user,
                                       const Channel& channel,
                                       const Server& server);

                static void whoReply(User& user,
                                     const Channel& channel,
                                     const Server& server);
                static void endOfwhoReply(User& user,
                                          const Channel& channel,
                                          const Server& server);

                static void inviting(User& user,
                                     const Server& server,
                                     const std::string& invitedUser,
                                     const Channel& channel);

                static void currUserModes(User& user, const Server& server);
                static void channelModeIs(User& user, const Channel& channel, const Server& server);
                static void creationTime(User& user, const Channel& channel, const Server& server);

                static void topic(User& user, const Channel& channel, const Server& server);
                static void topicWhoTime(User& user, const Channel& channel, const Server& server);
                static void noTopic(User& user, Channel& channel, const Server& server);

                static void listStart(User& user, const Server& server);
                static void list(User& user, Server& server);
                static void listEnd(User& user, Server& server);

                static void isOn(User& user, const std::vector<std::string>& nicknames, Server &server);
        };

        class Error {
            public:
                static void noSuchNick(User& user,
                                       const Server& server,
                                       const std::string& nickName);

                static void alreadyRegistered(User& user, const Server& server);

                static void needMoreParameters(User& user,
                                               const Server& server,
                                               const std::string& cmdName);
                static void erroneousNick(User& user,
                                          const Server& server,
                                          const std::string& newNickname);
                static void nickInUse(User& user,
                                      const Server& server,
                                      const std::string& newNickname);
                static void channelIsFull(User& user,
                                          const Server& server,
                                          const std::string& channelName);
                static void noNicknameGiven(User& user, const Server& server);

                static void noRecipient(User& user,
                                        const std::string& command,
                                        const Server& server);

                static void noTextToSend(User& user, const Server& server);

                static void noSuchChannel(User& user,
                                          const std::string& channel,
                                          const Server& server);

                static void notOnChannel(User& user,
                                         const Channel& channel,
                                         const Server& server);

                static void notRegistered(User& user, const Server& server);

                static void userDontMatchView(User& user, const Server& server);
                static void userDontMatchSet(User& user, const Server& server);

                static void chanOperPrivNeeded(User& user, const Channel& channel, const Server& server);

                static void userNotInChannel(User& user, const std::string& nickname, const Channel& channel, const Server& server);
                static void channelPrivilegesNeeded(User& user,
                                                    const Server& server,
                                                    const Channel& channel);

                static void userOnChannel(User& user,
                                          const Server& server,
                                          const std::string& invitedUser,
                                          const Channel& channel);

                static void unknownCommand(User& user,
                                           const Server& server,
                                           const std::string& command);

                static void badChannelMask(User& user,
                                           const Server& server,
                                           const std::string& channelName);

                static void tooManyChannels(User& user,
                                            const Server& server,
                                            const std::string& channelName);
                static void inviteOnlyChannel(User& user,
                                              const Server& server,
                                              const Channel& channel);

                static void badChannelKey(User& user,
                                          const Server& server,
                                          const Channel& channel);

                static void passwordMissMatch(User& user, const Server& server);
        };

    private:
        static std::string  _constructHeader(const std::string& numericID,
                                             const std::string& hostname);
};
