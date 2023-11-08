#include "NumericReplies.hpp"

NumericReplies::HeaderConstructor::HeaderConstructor(const std::string &number, const std::string &hostname) {
    _content << ":" << hostname << " " << number;
}

const std::stringstream &NumericReplies::HeaderConstructor::getContent() const {
    return _content;
}

std::ostream&   operator<<(std::ostream& os, const NumericReplies::HeaderConstructor& headerConstructor) {
    os << headerConstructor.getContent().str();
    return (os);
}

std::string NumericReplies::isAlreadyRegistered() {
    std::stringstream   reply;

    reply << HeaderConstructor(ERR_ALREADYREGISTERED, "127.0.0.1") << " :Unauthorized command (already registered)\r\n";
    return (reply.str());
}

std::string NumericReplies::notEnoughParameters(const std::string& cmdName) {
    std::stringstream   reply;

    reply << HeaderConstructor(ERR_NEEDMOREPARAMS, "127.0.0.1") << " * " << cmdName << " :Not enough parameters\r\n";
    return (reply.str());
}
