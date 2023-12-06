#pragma once

#include "ISocket.hpp"

#include <stdint.h>
#include <string>
#include <map>
#include <queue>
#include <sstream>

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

        void                    setNickName(const std::string& newNickName);
        const std::string&      getNickName() const;

        const std::string&  getUserName() const;

        const std::string&  getRealName() const;

        std::string getHostMask() const;

        static void initRequestsHandlers();

        void    handleEvent(uint32_t epollEvents, Server& server);

        bool    isRegistered() const;

        void    sendMessage(const std::string &message, const Server& server);
        void    sendMessageToConnections(const std::string& message, const Server& server);


        static std::string    defaultNickname;

    private:
        typedef void (User::*RequestHandler)(Server&, const std::vector<std::string>&);
        typedef std::map<std::string, RequestHandler>   RequestsHandlersMap;

        void    _flushMessages(Server& server);

        void        _handleEPOLLIN(Server& server);
        void        _processRequest(Server& server);
        void        _handleRequest(Server& server, const std::string& request);
        static bool _isCommandAllowedWhenNotRegistered(User::RequestHandler requestHandler);

        void    sendErrorAndDestroyUser(const std::string& message, Server& server);

        void    _handlePASS(Server& server, const std::vector<std::string>& args);
        void    _handleUSER(Server& server, const std::vector<std::string>& args);
        void    _handleNICK(Server& server, const std::vector<std::string>& args);
        void    _handlePRIVMSG(Server& server, const std::vector<std::string>& args);
        void    _handleJOIN(Server& server, const std::vector<std::string>& args);
        void    _handlePING(Server& server, const std::vector<std::string>& args);
        void    _handleWHO(Server& server, const std::vector<std::string>& args);
        void    _handlePART(Server& server, const std::vector<std::string>& args);
        void    _handleTOPIC(Server& server, const std::vector<std::string>& args);
        void    _handleMODE(Server& server, const std::vector<std::string>& args);
        void    _handleLIST(Server& server, const std::vector<std::string>& args);
        void    _handleISON(Server& server, const std::vector<std::string>& args);
        void    _handleINVITE(Server& server, const std::vector<std::string>& args);
        void    _handleKICK(Server& server, const std::vector<std::string>& args);

        void    _registerUserIfReady(Server& server);

        static RequestsHandlersMap _requestsHandlers;

        const int   _fd;

        JoinedChannelCounter    _nbOfJoinedLocalChannels;
        JoinedChannelCounter    _nbOfJoinedRegularChannels;

        bool        _isRegistered;

        std::string _nickName;
        std::string _userName;
        std::string _realName;
        bool        _passwordWasGiven;

        std::string _requestBuffer;

        std::queue<std::string> _messagesBuffer;

        bool    _shouldDestroyUserAfterFlush;
};
