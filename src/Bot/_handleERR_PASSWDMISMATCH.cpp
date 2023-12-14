#include "Bot.hpp"

#include <cstdlib>

void Bot::_handleERR_PASSWDMISMATCH(const ft::String& author,
                                    const std::vector<ft::String>& args) {
    (void) author;
    (void) args;
    ft::Log::error << "Bad password given" << std::endl;
    _connected = false;
}
