#include "Bot.hpp"

void Bot::_handleJOIN(const ft::String& author,
                      const std::vector<ft::String>& args) {
    if (author == _nicknames.front())
        return;
    ft::Log::info << author << " has joined " << args[0] << std::endl;
    _channels[args[0]].push_back(author);
    ft::Log::info << "Channel: " << args[0] << " contain " << _channels[args[0]] << " users" << std::endl;
}
