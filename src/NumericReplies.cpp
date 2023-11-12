#include "NumericReplies.hpp"
#include "User.hpp"

#include <fstream>

#define SERVER_NAME "127.0.0.1"
#define NETWORK_NAME "42IRC"
#define SERVER_VERSION "0.1"
#define CREATION_DATE "November the 9th of 2023"

#define PATH_TO_MOTD "data/MOTD.txt" // TODO this path only works if IRC binary
                                     // TODO is in the current working directory

// Reply

std::string NumericReplies::Reply::welcome(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_WELCOME, SERVER_NAME) << nickName
            << " :Welcome to the " NETWORK_NAME " Network, " << nickName << "\r\n";
    return reply.str();
}

std::string   NumericReplies::Reply::yourHost(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_YOURHOST, SERVER_NAME) << nickName
          << " :Your host is " SERVER_NAME ", running version " SERVER_VERSION << "\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::create(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_CREATE, SERVER_NAME) << nickName
            << " :This server was created on " CREATION_DATE "\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::myInfo(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_MYINFO, SERVER_NAME) << nickName << ' '
            << SERVER_NAME << ' ' << SERVER_VERSION << " _ itkol\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::iSupport(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_ISUPPORT, SERVER_NAME) << nickName << " "
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
    return reply.str();
}

std::string  NumericReplies::Reply::localUserClient(const std::string& nickName,
                                                    const size_t nbOfUsers) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LUSERCLIENT, SERVER_NAME) << nickName
            << " :There are " << nbOfUsers << " users and 0 invisible on 1 server\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::localUserChannels(const std::string& nickName,
                                                      const size_t nbOfChannels) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LUSERCHANNELS, SERVER_NAME) << nickName << ' '
            << nbOfChannels << " :channels formed\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::localUserMe(const std::string& nickName,
                                                const size_t nbOfUsers) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LUSERME, SERVER_NAME) << nickName
            << " :I have " << nbOfUsers << " clients and 0 servers\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::localUsers(const std::string& nickName,
                                               size_t nbOfUsers,
                                               size_t peakRegisteredUserCount) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_LOCALUSERS, SERVER_NAME) << nickName << ' '
            << nbOfUsers << ' ' << peakRegisteredUserCount
            << " :Current local users " << nbOfUsers << ", peak "
            << peakRegisteredUserCount << "\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::globalUsers(const std::string& nickName,
                                               size_t nbOfUsers,
                                               size_t peakRegisteredUserCount) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_GLOBALUSERS, SERVER_NAME) << nickName << ' '
            << nbOfUsers << ' ' << peakRegisteredUserCount
            << " :Current global users " << nbOfUsers << ", peak "
            << peakRegisteredUserCount << "\r\n";
    return reply.str();
}

std::string  NumericReplies::Reply::messageOfTheDay(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(RPL_MOTDSTART, SERVER_NAME) << nickName
            << " :- " SERVER_NAME " Message of the day - \r\n";

    std::ifstream   file(PATH_TO_MOTD);
    if (!file.is_open()) {
        reply << _constructHeader(RPL_MOTD, SERVER_NAME) << nickName
                << " :Failed to open file " PATH_TO_MOTD "\r\n";
    } else {
        std::string line;
        while (std::getline(file, line)) {
            reply << _constructHeader(RPL_MOTD, SERVER_NAME) << nickName
                    << " :" << line << "\r\n";
        }
    }

    reply << _constructHeader(RPL_ENDOFMOTD, SERVER_NAME) << nickName
           << " :End of MOTD command\r\n";
    return reply.str();
}

// Error

std::string NumericReplies::Error::alreadyRegistered(const std::string& nickName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_ALREADYREGISTERED, SERVER_NAME) << nickName
            << " :Unauthorized command (already registered)\r\n";
    return reply.str();
}

std::string NumericReplies::Error::needMoreParameters(const std::string& nickName,
                                                      const std::string& cmdName) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NEEDMOREPARAMS, SERVER_NAME) << nickName
            << ' ' << cmdName << " :Not enough parameters\r\n";
    return reply.str();
}

std::string NumericReplies::Error::erroneousNick(const std::string& currNickname,
                                                 const std::string& newNickname) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_ERRONEUSNICKNAME, SERVER_NAME) << currNickname
          << " " << newNickname << " :Erroneous Nickname\r\n";
    return reply.str();
}

std::string NumericReplies::Error::nickInUse(const std::string& currNickname,
                                             const std::string& newNickname) {
    std::stringstream   reply;

    reply << _constructHeader(ERR_NICKNAMEINUSE, SERVER_NAME) << currNickname
          << " " << newNickname << " :Nickname is already in use.\r\n";
    return reply.str();
}

// _constructHeader

std::string NumericReplies::_constructHeader(const std::string &requestID,
                                             const std::string &hostname) {
    std::stringstream   result;

    result << ':' << hostname << ' ' << requestID << ' ';
    return result.str();
}
