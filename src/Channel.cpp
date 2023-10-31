#include "Channel.hpp"

#include <limits>

Channel::Channel(const std::string& name,
                 const std::string& password,
                 const int creatorFD):
    _name(name),
    _password(password),
    _topic(""),
    _membersFDs(),
    _operatorsFDs(),
    _invitedUsersFDs(),
    _isInviteOnly(false),
    _userLimit(Channel::getMaxPossibleUserLimit())
{
    // TODO check name
    // if bad name throw
    _membersFDs.insert(creatorFD);
    _operatorsFDs.insert(creatorFD);
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
    return _membersFDs;
}

void    Channel::addMember(const int newMemberFD) throw (Channel::IsFull) {
    if (_membersFDs.size() >= _userLimit)
        throw (Channel::IsFull());
    _membersFDs.insert(newMemberFD);
    this->removeInvitedUser(newMemberFD);
}

void    Channel::removeMember(int memberFD) {
    _membersFDs.erase(memberFD);
}

bool    Channel::doesMemberExist(const int memberFD) {
    return _membersFDs.contains(memberFD);
}


const Channel::UserContainer&   Channel::getOperators() const {
    return _operatorsFDs;
}

bool    Channel::isOperator(const int memberFD) const {
    return _operatorsFDs.contains(memberFD);
}

void    Channel::addOperator(int newOperatorFD) {
    _operatorsFDs.insert(newOperatorFD);
}

void    Channel::removeOperator(int operatorFD) {
    _operatorsFDs.erase(operatorFD);
}


const Channel::UserContainer&   Channel::getInvitedUsers() const {
    return _invitedUsersFDs;
}

bool    Channel::wasUserInvited(const int userFD) const {
    return _invitedUsersFDs.contains(userFD);
}

void    Channel::addInvitedUser(int newInvitedUserFD) {
    _invitedUsersFDs.insert(newInvitedUserFD);
}

void    Channel::removeInvitedUser(int invitedUserFD) {
    _invitedUsersFDs.erase(invitedUserFD);
}

bool    Channel::isInviteOnly() const {
    return _isInviteOnly;
}

void    Channel::setIsInviteOnly(const bool isInviteOnly) {
    _isInviteOnly = isInviteOnly;
}


Channel::UserLimit  Channel::getUserLimit() const {
    return _userLimit;
}

void    Channel::setUserLimit(const Channel::UserLimit newUserLimit)
            throw (Channel::HasMoreUserThanNewLimit) {
    if (_membersFDs.size() >= newUserLimit)
        throw (Channel::HasMoreUserThanNewLimit()); // TODO need to check RFC to see expected behaviour
    _userLimit = newUserLimit;
}

void    Channel::removeUserLimit() {
    this->setUserLimit(Channel::getMaxPossibleUserLimit());
}

Channel::UserLimit  Channel::getMaxPossibleUserLimit() {
    return std::numeric_limits<Channel::UserLimit>::max();
}
