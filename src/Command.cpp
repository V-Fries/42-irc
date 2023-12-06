#include "Command.hpp"
#include "ft.hpp"

Command::Command(const ft::String &rawCommand) {
    ft::String::const_iterator it = rawCommand.begin();
    while (isspace(*it)) ++it;
    _command = ft::String::getFirstWord(it, rawCommand.end(), ": ");
    it += _command.length();
    while (isspace(*it)) ++it;
    ft::String::const_iterator currIterator = it;
    while (currIterator != rawCommand.end() && \
            *currIterator != ':')
            ++currIterator;
    _args = ft::String(it, currIterator).split(" ");
    if (*currIterator == ':') {
        ++currIterator;
        if (currIterator != rawCommand.end())
            _args.push_back(ft::String(currIterator, rawCommand.end()));
    }
}

const ft::String &Command::getCommand() const {
    return _command;
}

const std::vector<ft::String> &Command::getArgs() const {
    return _args;
}

std::ostream& operator<<(std::ostream& os, const Command& cmd) {
    os << "Command: " << cmd.getCommand() << " | ";
    os << "Args: " << cmd.getArgs() ;
    return os;
}
