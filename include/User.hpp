#pragma once

#include "ISocket.hpp"
#include "ft_String.hpp"

#include <stdint.h>
#include <map>
#include <queue>
#include <sstream>

#include "Channel.hpp"

class Server;

class User : public ISocket {
    public:
        typedef int JoinedChannelCounter;

        static const JoinedChannelCounter maxNbOfJoinedRegularChannels = 42; // # channels
        static const JoinedChannelCounter maxNbOfJoinedLocalChannels = 42; // & channels
        static const size_t maxNickNameLength = 42;

        explicit User(int fd);

        int getFD() const;

        bool    hasJoinedTheMaxNbOfRegularChannels() const;
        bool    hasJoinedTheMaxNbOfLocalChannels() const;

        void    setIsRegistered(bool isRegistered);

        void                setNickName(const ft::String& newNickName);
        const ft::String&   getNickName() const;

        const ft::String&   getUserName() const;

        const ft::String&   getRealName() const;

        ft::String getHostMask() const;

        static void initRequestsHandlers();

        void    handleEvent(uint32_t epollEvents, Server& server);

        bool    isRegistered() const;

        void    sendMessage(const ft::String &message, const Server& server);
        void    sendMessageToConnections(const ft::String& message,
                                         const Server& server);

        void    leaveChannel(const ft::String& channelName);

        static ft::String    defaultNickname;

    private:
        typedef void (User::*RequestHandler)(Server&, const std::vector<ft::String>&);
        typedef std::map<ft::String, RequestHandler>   RequestsHandlersMap;

        void    _flushMessages(Server& server);

        void        _handleEPOLLIN(Server& server);
        void        _processRequest(Server& server);
        void        _handleRequest(Server& server, const ft::String& request);
        static bool _isCommandAllowedWhenNotRegistered(User::RequestHandler requestHandler);

        void    sendErrorAndDestroyUser(const ft::String& message, Server& server);

        void    _handlePASS(Server& server, const std::vector<ft::String>& args);
        void    _handleUSER(Server& server, const std::vector<ft::String>& args);
        void    _handleNICK(Server& server, const std::vector<ft::String>& args);
        void    _handlePRIVMSG(Server& server, const std::vector<ft::String>& args);
        void    _handleJOIN(Server& server, const std::vector<ft::String>& args);
        void    _handlePING(Server& server, const std::vector<ft::String>& args);
        void    _handleWHO(Server& server, const std::vector<ft::String>& args);
        void    _handlePART(Server& server, const std::vector<ft::String>& args);
        void    _handleTOPIC(Server& server, const std::vector<ft::String>& args);
        void    _handleMODE(Server& server, const std::vector<ft::String>& args);
        void    _handleLIST(Server& server, const std::vector<ft::String>& args);
        void    _handleISON(Server& server, const std::vector<ft::String>& args);
        void    _handleINVITE(Server& server, const std::vector<ft::String>& args);
        void    _handleKICK(Server& server, const std::vector<ft::String>& args);
        void    _handleQUIT(Server& server, const std::vector<ft::String>& args);

        void    _registerUserIfReady(Server& server);

        static RequestsHandlersMap _requestsHandlers;

        const int   _fd;

        JoinedChannelCounter    _nbOfJoinedLocalChannels;
        JoinedChannelCounter    _nbOfJoinedRegularChannels;

        bool        _isRegistered;

        ft::String  _realName;
        ft::String  _nickName;
        ft::String  _userName;
        bool        _passwordWasGiven;

        ft::String _requestBuffer;
        size_t     _lastIndexOfBufferWithNoDelimiters;

        std::queue<ft::String> _messagesBuffer;

        bool    _shouldDestroyUserAfterFlush;

        std::map<ft::String, Channel*> _channels;
};
