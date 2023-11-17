#pragma once

#include "ISocket.hpp"
#include "Command.hpp"

#include <stdint.h>
#include <string>
#include <map>
#include <queue>

class Server;

class User : public ISocket {
    public:
        static const int maxNbOfJoinedRegularChannels = 42; // # channels
        static const int maxNbOfJoinedLocalChannels = 42; // & channels
        static const int maxNickNameLength = 42;

        explicit User(int fd);

        int                 getFD() const;
        void                setIsRegistered(bool isRegistered);
        const std::string&  getNickName() const;
        const std::string&  getUserName() const;
        const std::string&  getRealName() const;

        static void initRequestsHandlers();

        void    handleEvent(uint32_t epollEvents, Server& server);

        bool    isRegistered() const;

        void    sendMessage(const std::string &message, const Server& server);

        static std::string    defaultNickname;

    private:
        typedef void (User::*RequestHandler)(Server&, const std::vector<std::string>&);
        typedef std::map<std::string, RequestHandler>   RequestsHandlersMap;

        void    _flushMessages(Server& server);

        void    _handleEPOLLIN(Server& server);
        void    _processRequest(Server& server);
        void    _handleRequest(Server& server, const std::string& request);

        void    _handlePASS(Server& server, const std::vector<std::string>& args);
        void    _handleUSER(Server& server, const std::vector<std::string>& args);
        void    _handleNICK(Server& server, const std::vector<std::string>& args);
        void    _handlePRIVMSG(Server& server, const std::vector<std::string>& args);
        void    _handleJOIN(Server& server, const std::vector<std::string>& args);
        void    _handlePING(Server& server, const std::vector<std::string>& args);
        void    _handleWHO(Server& server, const std::vector<std::string>& args);

        void    _registerUserIfReady(Server& server);

        bool    _checkNickname(const std::string &nickName, const Server &server);

        static RequestsHandlersMap _requestsHandlers;

        const int   _fd;

        bool        _isRegistered;

        std::string _nickName;
        std::string _userName;
        std::string _realName;
        std::string _password;

        std::string _requestBuffer;

        std::queue<std::string> _messagesBuffer;
};
