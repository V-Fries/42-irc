#pragma once

#include "ft.hpp"
#include "Topic.hpp"

#include <string>

class Server;
class User;

#define MODE_INVITE_ONLY (1 << 0)
#define MODE_TOPIC_PROTECTED (1 << 1)
#define MODE_PASSWORD (1 << 2)
#define MODE_LIMIT (1 << 3)
#define MODE_OPERATOR (1 << 4)

class Channel {
    public:
        static ft::String    availableMhannelModes;

        class IncorrectName : public std::exception {};
        class IsFull : public std::exception {};
        class HasMoreUserThanNewLimit : public std::exception {};

        typedef ft::Set<User*>  UserContainer;
        typedef ft::Set<int>    UsersFdContainer;
        typedef void (Channel::*channelSetter)(uint8_t,
                                               ft::Vector<ft::String>::iterator&,
                                               User&,
                                               const Server&);

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
        bool                    isMember(int memberFD) const;
        bool                    isMember(const ft::String& nickname) const;
        bool                    isMember(const User* member) const;

        const UsersFdContainer& getOperators();
        ft::String              getNewOperators();
        size_t                  getNewOperatorsNumber() const;
        ft::String              getRemovedOperators();
        size_t                  getRemovedOperatorsNumber() const;
        bool                    isOperator(int memberFD) const;
        void                    addOperator(const User* newOperator);
        void                    addOperator(const ft::String& newOperatorNickname);
        void                    removeOperator(const User* operatorPtr);
        void                    removeOperator(int operatorFd);
        void                    removeOperator(const ft::String& operatorNickname);

        const UsersFdContainer& getInvitedUsers() const;
        bool                    wasUserInvited(int userFD) const;
        void                    addInvitedUser(int newInvitedUserFD);
        void                    removeInvitedUser(int invitedUserFD);
        bool                    isInviteOnly() const;

        uint8_t     getModes(uint8_t flags) const;
        uint8_t     getModes() const;
        void        addMode(uint8_t flag,
                            ft::Vector<ft::String>::iterator& arg,
                            User& author,
                            const Server& server);
        void        removeMode(uint8_t flag,
                               ft::Vector<ft::String>::iterator& arg,
                               User& author,
                               const Server& server);
        void        setMode(User& author,
                            char sign,
                            char modeChar,
                            ft::Vector<ft::String>::iterator& it,
                            ft::Vector<ft::String>::iterator end,
                            const Server& server);
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

        UserContainer           _members;
        UsersFdContainer        _operators;
        ft::Vector<ft::String>  _newOperators;
        ft::Vector<ft::String>  _removedOperators;

        UsersFdContainer    _invitedUsersFDs;

        size_t  _userLimit;

        time_t  _creationTine;
};
