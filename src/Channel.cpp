#include "Channel.hpp"
#include "ft_String.hpp"

#include <limits>
#include <sstream>

#include "ft_Log.hpp"

// public:

Channel::Channel(const std::string& name,
                 const std::string& password,
                 User *creator)
        throw (IncorrectName):
    _modes(0),
    _name(ft::String::toLower(name)),
    _password(password),
    _topic(),
    _members(),
    _operators(),
    _invitedUsersFDs(),
    _userLimit(Channel::getMaxPossibleUserLimit())
{
    if (!Channel::_isNameCorrect(_name)) throw (IncorrectName());

    ft::Log::info << "new channel: " << _name << std::endl;
    _members.insert(creator);
    _operators.insert(creator->getFD());
    if (_name[0] == '#')
        this->addModes(MODE_TOP);
}

const std::string&  Channel::getName() const {
    return _name;
}


const std::string&  Channel::getPassword() const {
    return _password;
}

void    Channel::setPassword(const std::string& newPassword) {
    _password = newPassword;
}


const Topic& Channel::getTopic() const {
    return _topic;
}

void    Channel::setTopic(const std::string& newTopic,
                          const std::string& author) {
    _topic.setContent(newTopic, author);
}


const Channel::UserContainer&   Channel::getMembers() const {
    return _members;
}

void    Channel::addMember(User *newMember) throw (Channel::IsFull) {
    if (_members.size() >= _userLimit) throw (Channel::IsFull());

    ft::Log::info << "channel " << _name << " add " << newMember->getNickName();
    _members.insert(newMember);
    this->removeInvitedUser(newMember->getFD());
}

void    Channel::removeMember(User *member) {
    this->removeOperator(member->getFD());
    _members.erase(member);
}

bool    Channel::isMember(const int memberFD) const {
    for (UserContainer::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if ((*it)->getFD() == memberFD)
            return (true);
    }
    return (false);
}

bool    Channel::isMember(const User* member) const {
    return _members.contains(const_cast<User*>(member));
}


const Channel::UsersFdContainer& Channel::getOperators() {
    return _operators;
}

bool    Channel::isOperator(const int memberFD) const {
    for (UsersFdContainer::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (*it == memberFD)
            return (true);
    }
    return (false);
}

void    Channel::addOperator(User *newOperator) {
    _operators.insert(newOperator->getFD());
}

void Channel::addOperator(int newOperatorFd) {
    _operators.insert(newOperatorFd);
}

void    Channel::removeOperator(User *operatorPtr) {
    _operators.erase(operatorPtr->getFD());
}

void Channel::removeOperator(int operatorFd) {
    _operators.erase(operatorFd);
}


const Channel::UsersFdContainer&   Channel::getInvitedUsers() const {
    return _invitedUsersFDs;
}

bool    Channel::wasUserInvited(const int userFD) const {
    return _invitedUsersFDs.contains(userFD);
}

void    Channel::addInvitedUser(const int newInvitedUserFD) {
    _invitedUsersFDs.insert(newInvitedUserFD);
}

void    Channel::removeInvitedUser(const int invitedUserFD) {
    _invitedUsersFDs.erase(invitedUserFD);
}

bool Channel::getModes(const uint8_t flags) const {
    return (_modes & flags);
}

void Channel::addModes(const uint8_t flags) {
    _modes |= flags;
}

void Channel::removeModes(const uint8_t flags) {
    _modes &= ~flags;
}

std::string Channel::modesString() const {
    std::stringstream   modesString;

    ft::Log::info << "channel " << _name << " raw modes: " << _modes << std::endl;
    modesString << "+";
    if (_modes & MODE_TOP)
        modesString << "t";
    if (_modes & MODE_INV)
        modesString << "i";
    if (_modes & MODE_KEY)
        modesString << "k";
    if (_modes & MODE_LIM)
        modesString << "l";
    return (modesString.str());
}

std::string Channel::modesArgs() const {
    std::stringstream   args;

    if (_modes & MODE_KEY)
        args << " " << _password;
    if (_modes & MODE_LIM)
        args << " " << _userLimit;
    return (args.str());
}

size_t  Channel::getUserLimit() const {
    return _userLimit;
}

void    Channel::setUserLimit(const size_t newUserLimit)
            throw (Channel::HasMoreUserThanNewLimit) {
    if (_members.size() >= newUserLimit) {
        throw (Channel::HasMoreUserThanNewLimit()); // TODO need to check RFC to see expected behaviour
    }

    _userLimit = newUserLimit;
}

size_t  Channel::getMaxPossibleUserLimit() {
    return std::numeric_limits<size_t>::max();
}

void Channel::sendMessage(const int senderFd, const std::string& message, const Server& server) {
    ft::Log::info << _name << " send message: " << message << std::endl;
    for(UserContainer::iterator it = _members.begin(); it != _members.end(); ++it) {
            if ((*it)->getFD() != senderFd) (*it)->sendMessage(message, server);
    }
}

// private:

bool    Channel::_isNameCorrect(const std::string& name) {
    if (name.length() == 0 || (name[0] != '#' && name[0] != '&')) {
        return false;
    }

    for (std::string::const_iterator it = name.begin() + 1; it != name.end(); ++it) {
        if (*it == ' ' || *it == ',' || *it == '\a') {
            return false;
        }
    }
    return true;
}
