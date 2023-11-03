#pragma once

#include "ISocket.hpp"
#include "EpollEvent.hpp"

#include <string>
#include <map>
#include <queue>

class Server;

class User : public ISocket {
    public:
        explicit User(int fd);

        int                 getFD() const;
        const std::string&  getNickName() const;
        const std::string&  getUserName() const;

        static void    initRequestsHandlers();

        void    handleEvent(uint32_t epollEvents, Server& server);
        void    sendMessage(const std::string &message);
        void    flushMessages(uint32_t epollEvents);

    private:
        typedef void (User::*RequestHandler)(Server&, const std::string&);
        typedef std::map<std::string, RequestHandler>   RequestsHandlersMap;

        void    _handleEPOLLIN(Server& server);
        void    _processRequest(Server& server);
        void    _handleRequest(Server& server, const std::string& request);

        void    _handlePASS(Server& server, const std::string& request);
        void    _handleUSER(Server& server, const std::string& request);
        void    _handleNICK(Server& server, const std::string& request);

        static RequestsHandlersMap _requestsHandlers;

        const int   _fd;
        bool        _isRegistered;

        std::string _nickName;
        std::string _userName;

        std::string _buffer;

        std::queue<std::string> _messagesBuffer;
};
