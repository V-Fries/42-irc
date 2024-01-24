#include "Bot.hpp"

void    Bot::_handlePART(const ft::String& author,
                         const std::vector<ft::String>& args) {
    ft::Log::info << author << " quit " << args[0] << std::endl;
    _removeSomeone(author, args[0]);
}
