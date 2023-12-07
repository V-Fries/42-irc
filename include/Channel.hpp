#pragma once

#include "ft_Set.hpp"
#include "ft_Exception.hpp"
#include "Topic.hpp"

#include <string>

class Server;
class User;


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

        Channel(const ft::String& name,
                const ft::String& password,
                User& creator)
            throw (IncorrectName);

        const ft::String&  getName() const;

        const ft::String&  getPassword() const;
        void                setPassword(const ft::String& newPassword);

        const Topic&    getTopic() const;
        void            setTopic(const ft::String& newTopic,
                                 const ft::String& author);

        const UserContainer&    getMembers() const;
        void                    addMember(User* newMember)
                                    throw (Channel::IsFull);
        void                    removeMember(User* member);
        bool                    isMember(int memberFD) const; // TODO remove this
        bool                    isMember(const ft::String& nickname) const;
        bool                    isMember(const User* member) const;

        const UsersFdContainer& getOperators();
        bool                    isOperator(int memberFD) const;
        void                    addOperator(User* newOperator);
        void                    addOperator(int newOperatorFd);
        void                    removeOperator(const User* operatorPtr);
        void                    removeOperator(int operatorFd);

        const UsersFdContainer& getInvitedUsers() const;
        bool                    wasUserInvited(int userFD) const;
        void                    addInvitedUser(int newInvitedUserFD);
        void                    removeInvitedUser(int invitedUserFD);
        bool                    isInviteOnly() const;

        bool        getModes(uint8_t flags) const;
        void        addModes(uint8_t flags);
        void        removeModes(uint8_t flags);
        ft::String  modesString() const;
        ft::String  modesArgs() const;

        size_t          getUserLimit() const;
        void            setUserLimit(size_t newUserLimit)
                            throw (Channel::HasMoreUserThanNewLimit);
        static size_t   getMaxPossibleUserLimit();

        time_t  getCreationTime() const;

        void    sendMessage(int senderFd,
                            const ft::String& message,
                            const Server& server);

    private:
        static bool _isNameCorrect(const ft::String& name);

        uint8_t _modes;

        const ft::String   _name;

        ft::String _password;

        Topic   _topic;

        UserContainer       _members;
        UsersFdContainer    _operators; // TODO use pointers

        UsersFdContainer    _invitedUsersFDs;

        size_t  _userLimit;

        time_t  _creationTine;
};
