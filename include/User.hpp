#pragma once

#include "ISocket.hpp"
#include "EpollEvent.hpp"

#include <string>
#include <map>

class Server;

class User : public ISocket {
    public:
        User(int fd,
             const std::string& nickName,
             const std::string& userName);

        int                 getFD() const;
        const std::string&  getNickName() const;
        const std::string&  getUserName() const;

        static void    initRequestsHandlers();

        void    handleEvent(uint32_t epollEvents, Server& server);

    private:
        typedef void (User::*RequestHandler)(Server&, const std::string&);
        typedef std::map<std::string, RequestHandler>   RequestsHandlersMap;

        void    _handleEPOLLIN(Server& server);
        void    _processRequest(Server& server);
        void    _redirectRequest(Server& server, const std::string& request);

        void    _handlePASS(Server& server, const std::string& request);
        void    _handleUSER(Server& server, const std::string& request);
        void    _handleNICK(Server& server, const std::string& request);

        static RequestsHandlersMap _requestsHandlers;

        const int           _fd;
        const std::string   _nickName;
        const std::string   _userName;
        std::string         _buffer;
};
