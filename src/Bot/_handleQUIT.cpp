#include "Bot.hpp"

void    Bot::_handleQUIT(const ft::String& author,
                         const std::vector<ft::String>& args) {
    ft::Log::info << author << " quit the server: " << args[0] << std::endl;
    _removeSomeone(author);
}
