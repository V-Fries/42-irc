#include "Bot.hpp"

void Bot::_handleRPL_NAMREPLY(const ft::String& author,
                              const std::vector<ft::String>& args) {
    (void) author;
    std::vector<ft::String> members = args[3].split(" ");

    for (std::vector<ft::String>::iterator it = members.begin();
         it != members.end();
         ++it) {
        if (*it->begin() == '@') {
            it->erase(it->begin());
        }
    }
    _channels[args[2]] = members;
    ft::Log::info << "Channel: " << args[2] << " contain " << _channels[args[2]] << " users" << std::endl;
}
