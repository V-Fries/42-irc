#include "Bot.hpp"

void Bot::_handleRPL_LIST(const ft::String& author,
                          const std::vector<ft::String>& args) {
    (void) author;
    ft::Log::info << "channel: " << args << std::endl;
    this->sendMessage("JOIN " + args[1] + "\r\n");
}
