#include "Channel.hpp"

#include <algorithm>
#include <cstdlib>

#include "ft.hpp"

#include <limits>
#include <sstream>
#include <ctime>

#include "ft_Log.hpp"
#include "NumericReplies.hpp"
#include "Server.hpp"
#include "User.hpp"

// public:

ft::String Channel::availableMhannelModes = "itkol";

Channel::Channel(const ft::String& name,
                 const ft::String& password,
                 User& creator)
        throw (IncorrectName):
    _modes(0),
    _name(name.copyToLower()),
    _password(password),
    _topic(),
    _members(),
    _operators(),
    _invitedUsersFDs(),
    _userLimit(Channel::getMaxPossibleUserLimit())
{
    if (!Channel::_isNameCorrect(_name)) throw (IncorrectName());

    ft::Log::info << "new channel: " << _name << std::endl;
    _members.insert(&creator);
    _operators.insert(creator.getFD());
    if (_name[0] == '#')
        _modes = MODE_TOPIC_PROTECTED;
    _creationTine = std::time(NULL);
}

const ft::String&  Channel::getName() const {
    return _name;
}

const ft::String&  Channel::getPassword() const {
    return _password;
}

void    Channel::setPassword(const ft::String& newPassword) {
    _password = newPassword;
}

const Topic& Channel::getTopic() const {
    return _topic;
}

void    Channel::setTopic(const ft::String& newTopic,
                          const ft::String& author) {
    _topic.setContent(newTopic, author);
}

const Channel::UserContainer&   Channel::getMembers() const {
    return _members;
}

void    Channel::addMember(User *newMember) throw (Channel::IsFull) {
    if (_members.size() >= _userLimit && (_modes & MODE_LIMIT)) {
        throw (Channel::IsFull());
    }

    ft::Log::info << "channel " << _name << " add " << newMember->getNickName();
    _members.insert(newMember);
    this->removeInvitedUser(newMember->getFD());
}

void    Channel::removeMember(User *member) {
    this->removeOperator(member);
    _members.erase(member);
    member->leaveChannel(_name);
}

bool    Channel::isMember(const int memberFD) const {
    for (UserContainer::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if ((*it)->getFD() == memberFD)
            return (true);
    }
    return (false);
}

bool Channel::isMember(const ft::String& nickname) const {
    for (UserContainer::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if ((*it)->getNickName() == nickname)
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

ft::String Channel::getNewOperators() {
    if (_newOperators.empty()) {
        return "";
    }

    ft::String  newOperators = _newOperators.join(" ");
    _newOperators.clear();
    return newOperators;
}

size_t Channel::getNewOperatorsNumber() const {
    return _newOperators.size();
}

ft::String Channel::getRemovedOperators() {
    if (_removedOperators.empty()) {
        return "";
    }

    ft::String  removedOperators = _removedOperators.join(" ");
    _removedOperators.clear();
    return removedOperators;
}

size_t Channel::getRemovedOperatorsNumber() const {
    return _removedOperators.size();
}

bool    Channel::isOperator(const int memberFD) const {
    for (UsersFdContainer::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (*it == memberFD)
            return (true);
    }
    return (false);
}

void    Channel::addOperator(const User *newOperator) {
    _operators.insert(newOperator->getFD());
}

void Channel::addOperator(const ft::String& newOperatorNickname) {
    UserContainer::iterator it;
    for (it = _members.begin();
         it != _members.end() && (*it)->getNickName() != newOperatorNickname;
         ++it) {}
    if ((*it)->getNickName() != newOperatorNickname) {
        return;
    }
    if (!_operators.contains((*it)->getFD())) {
        _newOperators.push_back(newOperatorNickname);
        _operators.insert((*it)->getFD());
    }
    _removedOperators.remove(newOperatorNickname);
}

void    Channel::removeOperator(const User *operatorPtr) {
    _operators.erase(operatorPtr->getFD());
}

void Channel::removeOperator(int operatorFd) {
    _operators.erase(operatorFd);
}

void Channel::removeOperator(const ft::String& operatorNickname) {
    UserContainer::iterator it;
    for (it = _members.begin();
         it != _members.end() && (*it)->getNickName() != operatorNickname;
         ++it) {}
    if ((*it)->getNickName() != operatorNickname) {
        return;
    }
    if (_operators.contains((*it)->getFD())) {
        _removedOperators.push_back(operatorNickname);
        _operators.erase((*it)->getFD());
    }
    _newOperators.remove(operatorNickname);
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

bool Channel::isInviteOnly() const {
    return _modes & MODE_INVITE_ONLY;
}

uint8_t Channel::getModes(const uint8_t flags) const {
    return (_modes & flags);
}

uint8_t Channel::getModes() const {
    return _modes;
}

void Channel::addMode(const uint8_t flag,
                      ft::Vector<ft::String>::iterator& arg,
                      User& author,
                      const Server&server) {
    ft::String  param = *arg;

    if (flag & MODE_PASSWORD) {
        ++arg;
        _password = *arg;
    }
    else if (flag & MODE_LIMIT) {
        ++arg;
        _userLimit = std::strtol(arg->c_str(), NULL, 10);
    }
    else if (flag & MODE_OPERATOR) {
        ++arg;
        ft::Log::debug << "add operator: " << *arg << std::endl;
        if (!server.nicknameIsTaken(*arg)) {
            NumericReplies::Error::noSuchNick(author, server, *arg);
            return;
        }
        if (!this->isMember(*arg)) {
            NumericReplies::Error::userNotInChannel(author, *arg, *this, server);
            return;
        }
        this->addOperator(*arg);
    }
    _modes |= flag;
}

void Channel::removeMode(const uint8_t flag,
                         ft::Vector<ft::String>::iterator& arg,
                         User& author,
                         const Server& server) {
    if (flag & MODE_PASSWORD) {
        ++arg;
        if (*arg != _password) {
            return;
        }
    }
    if (flag & MODE_OPERATOR) {
        ++arg;
        ft::Log::debug << "remove operator: " << *arg << std::endl;
        if (!server.nicknameIsTaken(*arg)) {
            NumericReplies::Error::noSuchNick(author, server, *arg);
            return;
        }
        if (!this->isMember(*arg)) {
            NumericReplies::Error::userNotInChannel(author, *arg, *this, server);
            return;
        }
        this->removeOperator(*arg);
    }
    _modes &= ~flag;
}

void Channel::setMode(User& author,
                      const char sign,
                      const char modeChar,
                      ft::Vector<ft::String>::iterator& it,
                      const ft::Vector<ft::String>::iterator end,
                      const Server& server) {
    channelSetter   setter = &Channel::addMode;

    if (it == end && ((modeChar == 'l' && sign == '+') || modeChar == 'k' || modeChar == 'o')) {
        return;
    }

    if (sign == '-') {
        setter = &Channel::removeMode;
    }

    switch (modeChar) {
        case 'i':
            (this->*setter)(MODE_INVITE_ONLY, it, author, server);
            break;
        case 't':
            (this->*setter)(MODE_TOPIC_PROTECTED, it, author, server);
            break;
        case 'k':
            (this->*setter)(MODE_PASSWORD, it, author, server);
            break;
        case 'l':
            (this->*setter)(MODE_LIMIT, it, author, server);
            break;
        case 'o':
            (this->*setter)(MODE_OPERATOR, it, author, server);
        default:
            break;
    }
}

ft::String Channel::modesString() const {
    std::stringstream   modesString;

    ft::Log::info << "channel " << _name << " raw modes: " << _modes << std::endl;
    modesString << "+";
    if (_modes & MODE_TOPIC_PROTECTED)
        modesString << "t";
    if (_modes & MODE_INVITE_ONLY)
        modesString << "i";
    if (_modes & MODE_PASSWORD)
        modesString << "k";
    if (_modes & MODE_LIMIT)
        modesString << "l";
    return (modesString.str());
}

ft::String Channel::modesArgs() const {
    std::stringstream   args;

    if (_modes & MODE_PASSWORD)
        args << " " << _password;
    if (_modes & MODE_LIMIT)
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

time_t Channel::getCreationTime() const {
    return _creationTine;
}

void Channel::sendMessage(const int senderFd, const ft::String& message, const Server& server) {
    ft::Log::info << _name << " send message: " << message << std::endl;
    for(UserContainer::iterator it = _members.begin(); it != _members.end(); ++it) {
        if ((*it)->getFD() != senderFd) (*it)->sendMessage(message, server);
    }
}

// private:

bool    Channel::_isNameCorrect(const ft::String& name) {
    if (name.empty() || (name[0] != '#' && name[0] != '&')) {
        return false;
    }

    for (ft::String::const_iterator it = name.begin() + 1; it != name.end(); ++it) {
        if (*it == ' ' || *it == ',' || *it == '\a') {
            return false;
        }
    }
    return true;
}