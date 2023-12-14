#pragma once
#include <map>
#include <queue>
#include <sys/epoll.h>

#include "ft.hpp"

#define EPOLL_DEFAULT ((EPOLLIN) | (EPOLLRDHUP) | (EPOLLET))

class Bot {
    public:
        Bot(const uint16_t port, const ft::String& password);
        ~Bot();

        void    run();

        void    initRequestsHandlers();
        void    initConnection();

        void sendMessage(const ft::String&message);


    private:
        ft::String  _serverPassword;

        typedef void (Bot::*RequestHandler)(const ft::String&, const std::vector<ft::String>&);
        typedef std::map<ft::String, RequestHandler>   RequestsHandlersMap;

        int _sockfd;
        int _epollFd;

        bool    _connected;

        size_t      _lastIndexOfBufferWithNoDelimiters;
        ft::String  _requestBuffer;

        RequestsHandlersMap  _requestsHandlers;

        void _processRequest();
        void _handleEPOLLIN();
        void _handleRequest(const ft::String& request);

        void _flushMessages();

        void _handlePRIVMSG(const ft::String& author, const std::vector<ft::String>& args);
        void _handleINVITE(const ft::String& author, const std::vector<ft::String>& args);
        void _handleERR_PASSWDMISMATCH(const ft::String& author, const std::vector<ft::String>& args);
        void _handleIGNORE(const ft::String& author, const std::vector<ft::String>& args);

        std::queue<ft::String>  _messagesBuffer;
};
