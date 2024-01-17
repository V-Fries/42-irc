#include <sstream>

#include "Bot.hpp"

void Bot::_handleINVITE(const ft::String& author,
                        const std::vector<ft::String>& args) {
	if (args.empty())
		return;
    ft::Log::info << "invite: " << args << " by " << author << std::endl;

    std::stringstream   reply;

    reply << "JOIN " << *(args.end() - 1) << "\r\n";
    this->sendMessage(reply.str());
}
