#include "Command.hpp"
#include "ft.hpp"

Command::Command(const std::string &rawCommand) {
    std::string::const_iterator it = rawCommand.begin();
    std::string::const_iterator currIterator;

    while (isspace(*it)) ++it;
    _command = ft::String::getFirstWord(it, rawCommand.end(), ' ');
    it += _command.length();
    currIterator = it;
    while (currIterator != rawCommand.end() && \
            *currIterator != ':')
            ++currIterator;
    _args = ft::String::split(std::string(it, currIterator), " ");
    if (*currIterator == ':') {
        ++currIterator;
        _args.push_back(std::string(currIterator, rawCommand.end()));
    }
}

const std::string &Command::getCommand() const {
    return _command;
}

const std::vector<std::string> &Command::getArgs() const {
    return _args;
}

std::ostream& operator<<(std::ostream& os, const Command& cmd) {
    os << "Command: " << cmd.getCommand() << " | ";
    os << "Args: ";
    std::vector<std::string>::const_iterator    lastCommand = cmd.getArgs().end() - 1;
    std::vector<std::string>::const_iterator    it;
    for (it = cmd.getArgs().begin();
         it != lastCommand;
         ++it) {
        os << *it << " |&| ";
    }
    os << *it;
    return os;
}