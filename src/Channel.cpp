#include "Channel.hpp"
#include "ft_String.hpp"

#include <limits>

#include "ft_Log.hpp"

// public:

Channel::Channel(const std::string& name,
                 const std::string& password,
                 User *creator)
        throw (IncorrectName):
    _name(ft::String::toLower(name)),
    _password(password),
    _topic(""),
    _members(),
    _operators(),
    _invitedUsersFDs(),
    _isInviteOnly(false),
    _userLimit(Channel::getMaxPossibleUserLimit())
{
    if (!Channel::_isNameCorrect(_name)) throw (IncorrectName());

    ft::Log::info << "new channel: " << _name << std::endl;
    _members.insert(creator);
    _operators.insert(creator->getFD());
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


const std::string&  Channel::getTopic() const {
    return _topic;
}

void    Channel::setTopic(const std::string& newTopic) {
    _topic = newTopic;
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
    _members.erase(member);
}

bool    Channel::doesMemberExist(const int memberFD) {
    for (UserContainer::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if ((*it)->getFD() == memberFD)
            return (true);
    }
    return (false);
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

bool    Channel::isInviteOnly() const {
    return _isInviteOnly;
}

void    Channel::setIsInviteOnly(const bool isInviteOnly) {
    _isInviteOnly = isInviteOnly;
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

void    Channel::removeUserLimit() {
    this->setUserLimit(Channel::getMaxPossibleUserLimit());
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
