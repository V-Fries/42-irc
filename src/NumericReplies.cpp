#include "NumericReplies.hpp"
#include "User.hpp"
#include "Server.hpp"
#include "Channel.hpp"

#include <fstream>

#include "ft_Log.hpp"

#define NETWORK_NAME "42IRC"
#define SERVER_VERSION "0.1"
#define CREATION_DATE "November the 9th of 2023"

#define PATH_TO_MOTD "data/MOTD.txt" // TODO this path only works if IRC binary
                                     // TODO is in the current working directory

// Reply

void    NumericReplies::Reply::welcome(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_WELCOME, SERVER_NAME) << user.getNickName()
            << " :Welcome to the " NETWORK_NAME " Network, " << user.getNickName()
            << "\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::yourHost(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_YOURHOST, SERVER_NAME) << user.getNickName()
          << " :Your host is " SERVER_NAME ", running version " SERVER_VERSION
          << "\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::create(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_CREATE, SERVER_NAME) << user.getNickName()
            << " :This server was created on " CREATION_DATE "\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::myInfo(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_MYINFO, SERVER_NAME) << user.getNickName() << ' '
            << SERVER_NAME << ' ' << SERVER_VERSION << " _ itkol\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::iSupport(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_ISUPPORT, SERVER_NAME) << user.getNickName() << " "
            "CASEMAPPING=ascii "
            "CHANTYPES=#& "
            "CHANLIMIT=#:" << User::maxNbOfJoinedRegularChannels << ",&:"
                << User::maxNbOfJoinedLocalChannels << " "
            "CHANMODES=,k,l,it "
            "ELIST= "
            "NICKLEN=" << User::maxNickNameLength << " "
            ""
            ":are supported by this server\r\n"; // TODO we probably need to add
                                                 // TODO more things here
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::localUserClient(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LUSERCLIENT, SERVER_NAME) << user.getNickName()
            << " :There are " << server.getNbOfRegisteredUsers() << " users and "
               "0 invisible on 1 server\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::localUserChannels(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LUSERCHANNELS, SERVER_NAME) << user.getNickName()
            << ' ' << server.getNbOfChannels() << " :channels formed\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::localUserMe(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LUSERME, SERVER_NAME) << user.getNickName()
            << " :I have " << server.getNbOfRegisteredUsers() << " clients and 0"
               " servers\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::localUsers(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LOCALUSERS, SERVER_NAME) << user.getNickName()
            << ' ' << server.getNbOfRegisteredUsers() << ' '
            << server.getPeakRegisteredUserCount() << " :Current local users "
            << server.getNbOfRegisteredUsers() << ", peak "
            << server.getPeakRegisteredUserCount() << "\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::globalUsers(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_GLOBALUSERS, SERVER_NAME) << user.getNickName()
            << ' ' << server.getNbOfRegisteredUsers() << ' '
            << server.getPeakRegisteredUserCount() << " :Current global users "
            << server.getNbOfRegisteredUsers() << ", peak "
            << server.getPeakRegisteredUserCount() << "\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Reply::messageOfTheDay(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_MOTDSTART, SERVER_NAME) << user.getNickName()
            << " :- " SERVER_NAME " Message of the day - \r\n";

    std::ifstream   file(PATH_TO_MOTD);
    if (!file.is_open()) {
        reply << _constructHeader(RPL_MOTD, SERVER_NAME) << user.getNickName()
                << " :Failed to open file " PATH_TO_MOTD "\r\n";
    } else {
        ft::String line;
        while (std::getline(file, line)) {
            reply << _constructHeader(RPL_MOTD, SERVER_NAME) << user.getNickName()
                    << " :" << line << "\r\n";
        }
    }

    reply << _constructHeader(RPL_ENDOFMOTD, SERVER_NAME) << user.getNickName()
           << " :End of MOTD command\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::namesReply(User& user,
                                       const Channel& channel,
                                       const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_NAMREPLY, SERVER_NAME)
          << user.getNickName() << " = " << channel.getName() << " :";
    for (Channel::UserContainer::const_iterator it = channel.getMembers().begin();
         it != channel.getMembers().end();
         ++it) {
        if (it != channel.getMembers().end()) {
            if (channel.isOperator((*it)->getFD()))
                reply << "@";
            reply << (*it)->getNickName() << " ";
        }
    }
    reply << "\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::endOfNames(User& user,
                                       const Channel& channel,
                                       const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_ENDOFNAMES, SERVER_NAME) << user.getNickName()
          << " " << channel.getName() << " :End of NAMES list.\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::whoReply(User& user,
                                     const Channel& channel,
                                     const Server& server) {
    std::stringstream   reply;
    std::stringstream   replyLineStart;

    replyLineStart << _constructHeader(RPL_WHOREPLY, SERVER_NAME)
                   << user.getNickName() << " " << channel.getName() << " ";
    for (Channel::UserContainer::iterator it = channel.getMembers().begin();
         it != channel.getMembers().end();
         ++it) {
        ft::Log::info << "add " << (*it)->getNickName() << " to WHO list reply"
                        << std::endl;
        reply.str("");
        reply << replyLineStart.str() << (*it)->getUserName() << " "
             << "hostname " << SERVER_NAME << " " << (*it)->getNickName() << " H";
        if (channel.isOperator((*it)->getFD())) reply << "@";
        reply << " :0 " << (*it)->getRealName() << "\r\n";
        user.sendMessage(reply.str(), server);
    }
}

void NumericReplies::Reply::endOfwhoReply(User& user,
                                          const Channel& channel,
                                          const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_ENDOFWHO, SERVER_NAME) << user.getNickName()
            << " " << channel.getName() << " :End of WHO list.\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::currUserModes(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_UMODEIS, SERVER_NAME)
          << user.getNickName() << " +\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::channelModeIs(User& user, const Channel& channel, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_CHANNELMODEIS, SERVER_NAME)
          << user.getNickName() << " " << channel.getName() << " " << channel.modesString();
    if (channel.isMember(user.getFD()))
        reply << channel.modesArgs();
    reply << "\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::creationTime(User& user,
                                         const Channel& channel,
                                         const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_CREATIONTIME, SERVER_NAME)
          << user.getNickName() << " "
          << channel.getName() << " "
          << channel.getCreationTime() << "\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::inviting(User& user,
                                     const Server& server,
                                     const ft::String& invitedUser,
                                     const Channel& channel) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_INVITING, SERVER_NAME) << user.getNickName()
            << ' ' << invitedUser << ' ' << channel.getName() << "\r\n";
    user.sendMessage(reply.str(), server);
}

// Error

void NumericReplies::Error::noSuchNick(User& user,
                                       const Server& server,
                                       const ft::String& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NOSUCHNICK, SERVER_NAME) << user.getNickName()
            << ' ' << nickName << " :No such nick/channel\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::noRecipient(User& user,
                                        const ft::String& command,
                                        const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NORECIPIENT, SERVER_NAME)
          << user.getNickName() << " :No recipient given (" << command << ")\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::noTextToSend(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NOTEXTTOSEND, SERVER_NAME)
          << user.getNickName() << " :No text to send\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::noSuchChannel(User& user,
                                          const ft::String& channel,
                                          const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NOSUCHCHANNEL, SERVER_NAME)
          << user.getNickName() << " " << channel << " :No such channel\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::notOnChannel(User& user,
                                         const Channel& channel,
                                         const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NOTONCHANNEL, SERVER_NAME)
          << user.getNickName() << " " << channel.getName()
          << " :You're not on that channel\r\n";
    user.sendMessage(reply.str(), server);
}

// Error

void NumericReplies::Error::userDontMatchSet(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_USERSDONTMATCH, SERVER_NAME)
          << user.getNickName() << " :Can't change mode for other users\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::chanOperPrivNeeded(User& user,
                                               const Channel& channel,
                                               const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_CHANOPRIVSNEEDED, SERVER_NAME)
          << user.getNickName() << " "
          << channel.getName() << " :You're not channel operator\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::userNotInChannel(User& user,
                                             const ft::String& nickname,
                                             const Channel& channel,
                                             const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_USERNOTINCHANNEL, SERVER_NAME)
          << user.getNickName() << " "
          << nickname << channel.getName()
          << " :They aren't on that channel\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::topic(User& user, const Channel& channel, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_TOPIC, SERVER_NAME)
          << user.getNickName() << " " << channel.getName()
          << " :" << channel.getTopic().getContent() << "\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::topicWhoTime(User& user,
                                         const Channel& channel,
                                         const Server& server) {
    std::stringstream   reply;

    if (channel.getTopic().getSetAt().empty())
        return;
    reply << _constructHeader(RPL_TOPICWHOTIME, SERVER_NAME)
          << user.getNickName() << " "
          << channel.getName() << " "
          << channel.getTopic().getNickname() << " "
          << channel.getTopic().getSetAt() << "\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::noTopic(User& user, Channel& channel, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_NOTOPIC, SERVER_NAME)
          << user.getNickName() << " " << channel.getName()
          << " :No topic is set\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::listStart(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LISTSTART, SERVER_NAME)
          << user.getNickName() << " Channel :Users  Name\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::list(User& user, Server& server) {
    std::stringstream   lineStart;

    lineStart << _constructHeader(RPL_LIST, SERVER_NAME)
              << user.getNickName() << " ";
    for (Server::ChannelMap::const_iterator it = server.getChannels().begin();
         it != server.getChannels().end();
         ++it) {
        std::stringstream   reply;
        reply << lineStart.str()
              << it->first << " "
              << it->second->getMembers().size() << " :"
              << it->second->getTopic().getContent() << "\r\n";
        user.sendMessage(reply.str(), server);
    }
}

void NumericReplies::Reply::listEnd(User& user, Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LISTEND, SERVER_NAME)
          << user.getNickName() << " :End of /LIST\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Reply::isOn(User& user,
                                 const std::vector<ft::String>& nicknames,
                                 Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_ISON, SERVER_NAME)
          << user.getNickName() << " :";
    for (std::vector<ft::String>::const_iterator it = nicknames.begin();
         it != nicknames.end();
         ++it) {
        if (server.getUserByNickname(*it))
            reply << *it << " ";
    }
    reply << "\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::userDontMatchView(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_USERSDONTMATCH, SERVER_NAME)
          << user.getNickName() << " :Can't view modes for other users\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::alreadyRegistered(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_ALREADYREGISTERED, SERVER_NAME) << user.getNickName()
          << " :Unauthorized command (already registered)\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::needMoreParameters(User& user, const Server& server,
                                                  const ft::String& cmdName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NEEDMOREPARAMS, SERVER_NAME) << user.getNickName()
            << ' ' << cmdName << " :Not enough parameters\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::erroneousNick(User& user, const Server& server,
                                             const ft::String& newNickname) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_ERRONEUSNICKNAME, SERVER_NAME)
            << user.getNickName() << " " << newNickname
            << " :Erroneous Nickname\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::nickInUse(User& user, const Server& server,
                                         const ft::String& newNickname) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NICKNAMEINUSE, SERVER_NAME)
            << user.getNickName() << " " << newNickname
            << " :Nickname is already in use.\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::channelIsFull(User& user,
                                          const Server& server,
                                          const ft::String& channelName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_CHANNELISFULL, SERVER_NAME)
          << user.getNickName() << " " << channelName << " :Cannot join channel (+l)\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::noNicknameGiven(User &user, const Server &server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NONICKNAMEGIVEN, SERVER_NAME) << user.getNickName()
          << " :No nickname given\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::notRegistered(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NOTREGISTERED, SERVER_NAME) << user.getNickName()
            << " :You have not registered\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::channelPrivilegesNeeded(User& user,
                                                    const Server& server,
                                                    const Channel& channel) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_CHANOPRIVSNEEDED, SERVER_NAME) << user.getNickName()
            << ' ' << channel.getName() << " :You're not channel operator\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::userOnChannel(User& user,
                                          const Server& server,
                                          const ft::String& invitedUser,
                                          const Channel& channel) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_USERONCHANNEL, SERVER_NAME) << user.getNickName()
            << ' ' << invitedUser << ' ' << channel.getName() << " :is already on channel\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::unknownCommand(User& user,
                                              const Server& server,
                                              const ft::String& command) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_UNKNOWNCOMMAND, SERVER_NAME) << user.getNickName()
            << ' ' << command << " :Unknown command\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::badChannelMask(User& user,
                                              const Server& server,
                                              const ft::String& channelName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_USERONCHANNEL, SERVER_NAME) << channelName
            << " :Bad Channel Mask\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::tooManyChannels(User& user,
                                            const Server& server,
                                            const ft::String& channelName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_TOOMANYCHANNELS, SERVER_NAME) << user.getNickName()
            << ' ' << channelName << " :You have joined too many channels\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::inviteOnlyChannel(User& user,
                                                 const Server& server,
                                                 const Channel& channel) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_INVITEONLYCHAN, SERVER_NAME) << user.getNickName()
            << ' ' << channel.getName() << " :Cannot join channel (+i)\r\n";
    user.sendMessage(reply.str(), server);
}

void    NumericReplies::Error::badChannelKey(User& user,
                                             const Server& server,
                                             const Channel& channel) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_BADCHANMASK, SERVER_NAME) << user.getNickName()
            << ' ' << channel.getName() << " :Cannot join channel (+k)\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::passwordMissMatch(User& user, const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_PASSWDMISMATCH, SERVER_NAME) << user.getNickName()
            << " :Password incorrect\r\n";
    user.sendMessage(reply.str(), server);
}

void NumericReplies::Error::unknowMode(User& user,
                                       const char modeChar,
                                       const Server& server) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_UNKNOWNMODE, SERVER_NAME)
          << user.getNickName() << " " << modeChar << " :is unknown mode char to me\r\n";

    user.sendMessage(reply.str(), server);
}

// _constructHeader

ft::String NumericReplies::_constructHeader(const ft::String &numericID,
                                            const ft::String &hostname) {
    std::stringstream   result;

    result << ':' << hostname << ' ' << numericID << ' ';
    return result.str();
}
