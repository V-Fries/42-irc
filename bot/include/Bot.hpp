#pragma once
#include <map>
#include <queue>
#include <sys/epoll.h>
#include <algorithm>
#include <sstream>

#include "ft.hpp"

#define PATH_TO_WORDS "./words.csv"
#define EPOLL_DEFAULT ((EPOLLIN) | (EPOLLRDHUP) | (EPOLLET))

class Bot {
    public:
        Bot(const ft::String& ServerIp,
            uint16_t port,
            const ft::String& password,
            const std::vector<std::pair<ft::String, ft::String> >& kickWords);

        ~Bot();

        void    run();

        void    initRequestsHandlers();
        void    initConnection();

        void    sendMessage(const ft::String& message);

        void    addNickname(const ft::String& nickname);
        void    addNicknames(const std::vector<ft::String>& nickname);

    private:
        ft::String              _serverPassword;
        std::queue<ft::String>  _nicknames;

        typedef void (Bot::*RequestHandler)(const ft::String&,
                                            const std::vector<ft::String>&);
        typedef std::map<ft::String, RequestHandler>   RequestsHandlersMap;

        int _sockfd;
        int _epollFd;

        bool    _connected;
        bool    _registered;

        size_t      _lastIndexOfBufferWithNoDelimiters;
        ft::String  _requestBuffer;

        RequestsHandlersMap  _requestsHandlers;

        void _processRequest();
        void _handleEPOLLIN();
        void _handleRequest(const ft::String& request);

        void _flushMessages();

        void    _handleWELCOME(const ft::String& author, const std::vector<ft::String>& args);
        void    _handlePRIVMSG(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleINVITE(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleERR_PASSWDMISMATCH(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleERROR(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleNICKERRORS(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleIGNORE(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleRPL_LIST(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleJOIN(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleRPL_NAMREPLY(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleQUIT(const ft::String& author, const std::vector<ft::String>& args);
        void    _handleKICK(const ft::String& author, const std::vector<ft::String>& args);
        void    _handlePART(const ft::String& author, const std::vector<ft::String>& args);

        void    _removeSomeone(const ft::String& nickname);
        void    _removeSomeone(const ft::String& nickname, const ft::String& channelName);

        std::vector< std::pair<ft::String, ft::String> >  _kickWords;

        std::queue<ft::String>  _messagesBuffer;

        std::map< ft::String, std::vector<ft::String> >    _channels;
};
