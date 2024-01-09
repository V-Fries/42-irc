#include "Bot.hpp"

void    Bot::_handleNICKERRORS(const ft::String& author,
                               const std::vector<ft::String>& args) {
    (void) author;

    ft::Log::warning << "Nickname error: ";
    if (args.size() >= 3) {
        ft::Log::warning << *(args.begin() + 1) << " :";
    }
    ft::Log::warning << *(args.end() - 1) << std::endl;
    _nicknames.pop();
    if (_nicknames.empty()) {
        ft::Log::error << "No valid nickname given" << std::endl;
        _connected = false;
        return;
    }
    this->sendMessage("NICK " + _nicknames.front() + "\r\n");
}
