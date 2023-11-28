#pragma once

#include "ft_Set.hpp"

#include <string>

#include "Topic.hpp"
#include "User.hpp"

#define MODE_INV (1 << 0)
#define MODE_TOP (1 << 1)
#define MODE_KEY (1 << 2)
#define MODE_LIM (1 << 3)

class Channel {
    public:
        class IncorrectName : public std::exception {};
        class IsFull : public std::exception {};
        class HasMoreUserThanNewLimit : public std::exception {};

        typedef ft::Set<User*>  UserContainer;
        typedef ft::Set<int>    UsersFdContainer;

        Channel(const std::string& name,
                const std::string& password,
                User *creator)
            throw (IncorrectName);

        const std::string&  getName() const;

        const std::string&  getPassword() const;
        void                setPassword(const std::string& newPassword);

        const Topic& getTopic() const;
        void                setTopic(const std::string& newTopic, const std::string& author);

        const UserContainer&    getMembers() const;
        void                    addMember(User* newMember)
                                    throw (Channel::IsFull);
        void                    removeMember(User* member);
        bool                    isMember(int memberFD) const; // TODO remove this
        bool                    isMember(const User* member) const;

        const UsersFdContainer& getOperators();
        bool                    isOperator(int memberFD) const;
        void                    addOperator(User* newOperator);
        void                    addOperator(int newOperatorFd);
        void                    removeOperator(User* operatorPtr);
        void                    removeOperator(int operatorFd);

        const UsersFdContainer& getInvitedUsers() const;
        bool                    wasUserInvited(int userFD) const;
        void                    addInvitedUser(int newInvitedUserFD);
        void                    removeInvitedUser(int invitedUserFD);

        bool        getModes(uint8_t flags) const;
        void        addModes(uint8_t flags);
        void        removeModes(uint8_t flags);
        std::string modesString() const;
        std::string modesArgs() const;

        size_t          getUserLimit() const;
        void            setUserLimit(size_t newUserLimit)
                            throw (Channel::HasMoreUserThanNewLimit);
        static size_t   getMaxPossibleUserLimit();

        time_t  getCreationTime() const;

        void    sendMessage(int senderFd, const std::string& message, const Server& server);

    private:
        static bool _isNameCorrect(const std::string& name);

        uint8_t _modes;

        const std::string   _name;

        std::string _password;

        Topic   _topic;

        UserContainer       _members;
        UsersFdContainer    _operators; // TODO use pointers

        UsersFdContainer    _invitedUsersFDs;

        size_t  _userLimit;

        time_t  _creationTine;
};
