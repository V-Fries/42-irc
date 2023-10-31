#pragma once

#include "ft_Set.hpp"

#include <string>

class Channel {
    public:
        class IncorrectName : public std::exception {};
        class IsFull : public std::exception {};
        class HasMoreUserThanNewLimit : public std::exception {};

        typedef ft::Set<int>    UserContainer;
        typedef int             UserLimit;

        Channel(const std::string& name,
                const std::string& password,
                int creatorFD)
            throw (IncorrectName);

        const std::string&  getName() const;

        const std::string&  getPassword() const;
        void                setPassword(const std::string& newPassword);

        const std::string&  getTopic() const;
        void                setTopic(const std::string& newTopic);

        const UserContainer&    getMembers() const;
        void                    addMember(int newMemberFD)
                                    throw (Channel::IsFull);
        void                    removeMember(int memberFD);
        bool                    doesMemberExist(int memberFD);

        const UserContainer&    getOperators() const;
        bool                    isOperator(int memberFD) const;
        void                    addOperator(int newOperatorFD);
        void                    removeOperator(int operatorFD);

        const UserContainer&    getInvitedUsers() const;
        bool                    wasUserInvited(int userFD) const;
        void                    addInvitedUser(int newInvitedUserFD);
        void                    removeInvitedUser(int invitedUserFD);
        bool                    isInviteOnly() const;
        void                    setIsInviteOnly(bool isInviteOnly);

        UserLimit           getUserLimit() const;
        void                setUserLimit(UserLimit newUserLimit)
                                throw (Channel::HasMoreUserThanNewLimit);
        void                removeUserLimit();
        static UserLimit    getMaxPossibleUserLimit();

    private:
        static bool         _isNameCorrect(const std::string& name);
        const std::string   _name;

        std::string         _password;

        std::string         _topic;

        UserContainer   _membersFDs;
        UserContainer   _operatorsFDs;

        UserContainer   _invitedUsersFDs;
        bool            _isInviteOnly;

        UserLimit           _userLimit;
};
