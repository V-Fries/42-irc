#include "Bot.hpp"

void    Bot::_handleKICK(const ft::String& author,
                         const std::vector<ft::String>& args) {
	if (args.size() != 2)
		return;
    ft::Log::info << author << " kicked " << args[1] << std::endl;
    _removeSomeone(args[1], args[0]);
    ft::Log::info << "Channel: " << args[0] << " contain " << _channels[args[0]] << " users" << std::endl;
}
