#include "Bot.hpp"

void Bot::_handleERROR(const ft::String& author,
                       const std::vector<ft::String>& args) {
    (void) author;
	if (args.empty())
		return;
    ft::Log::error << "Server error: " << *args.begin() << std::endl;
}
