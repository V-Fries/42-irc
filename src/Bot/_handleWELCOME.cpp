#include "Bot.hpp"

void Bot::_handleWELCOME(const ft::String& author,
                         const std::vector<ft::String>& args) {
    (void) author;
    (void) args;
    _registered = true;
    this->sendMessage("LIST\r\n");
}
