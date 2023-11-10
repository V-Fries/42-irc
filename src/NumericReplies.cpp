#include "NumericReplies.hpp"

NumericReplies::HeaderConstructor::HeaderConstructor(const std::string &number,
                                                     const std::string &hostname) {
    _content << ':' << hostname << ' ' << number << ' ';
}

const std::stringstream &NumericReplies::HeaderConstructor::getContent() const {
    return _content;
}

std::ostream&   operator<<(std::ostream& os,
                           const NumericReplies::HeaderConstructor& headerConstructor) {
    os << headerConstructor.getContent().str();
    return (os);
}

std::string NumericReplies::Error::alreadyRegistered(const std::string& nickName) {
    std::stringstream   reply;

    reply << HeaderConstructor(ERR_ALREADYREGISTERED, "127.0.0.1") << nickName
            << " :Unauthorized command (already registered)\r\n";
    return (reply.str());
}

std::string NumericReplies::Error::needMoreParameters(const std::string& nickName,
                                                      const std::string& cmdName) {
    std::stringstream   reply;

    reply << HeaderConstructor(ERR_NEEDMOREPARAMS, "127.0.0.1") << nickName
            << ' ' << cmdName << " :Not enough parameters\r\n";
    return (reply.str());
}

std::string NumericReplies::Error::erroneousNick(const std::string& currNickname, const std::string& newNickname) {
    std::stringstream   reply;

    reply << HeaderConstructor(ERR_ERRONEUSNICKNAME, "127.0.0.1") << currNickname
          << " " << newNickname << " :Erroneous Nickname\r\n";
    return (reply.str());
}

std::string NumericReplies::Error::nickInUse(const std::string& currNickname, const std::string& newNickname) {
    std::stringstream   reply;

    reply << HeaderConstructor(ERR_NICKNAMEINUSE, "127.0.0.1") << currNickname
          << " " << newNickname << " :Nickname is already in use.\r\n";
    return (reply.str());
}
