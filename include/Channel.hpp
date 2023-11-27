#pragma once

#include "ft_Set.hpp"

#include <string>

#include "User.hpp"

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

        const std::string&  getTopic() const;
        void                setTopic(const std::string& newTopic);

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

        const UsersFdContainer&    getInvitedUsers() const;
        bool                       wasUserInvited(int userFD) const;
        void                       addInvitedUser(int newInvitedUserFD);
        void                       removeInvitedUser(int invitedUserFD);
        bool                       isInviteOnly() const;
        void                       setIsInviteOnly(bool isInviteOnly);

        size_t          getUserLimit() const;
        void            setUserLimit(size_t newUserLimit)
                            throw (Channel::HasMoreUserThanNewLimit);
        void            removeUserLimit();
        static size_t   getMaxPossibleUserLimit();

        void            sendMessage(int senderFd, const std::string& message, const Server& server);

    private:
        static bool _isNameCorrect(const std::string& name);


        const std::string   _name;

        std::string _password;

        std::string _topic;

        UserContainer       _members;
        UsersFdContainer    _operators; // TODO use pointers

        UsersFdContainer    _invitedUsersFDs; // TODO use pointers
        bool                _isInviteOnly;

        size_t  _userLimit;
};
