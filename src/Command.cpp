#include "Command.hpp"
#include "ft.hpp"

Command::Command(const std::string &rawCommand) {
    std::string::iterator   it;
    std::string::iterator   currIterator;
    std::string             copy;

    copy = std::string(rawCommand);
    if (copy.length() > 1 && (*(copy.end() - 1) == '\n')) {
        copy.erase(copy.end() - 1, copy.end());
    }
    it = copy.begin();
    while (isspace(*it)) ++it;
    _command = ft::String::getFirstWord(it, copy.end(), ": ");
    it += _command.length();
    while (isspace(*it)) ++it;
    currIterator = it;
    while (currIterator != copy.end() && \
            *currIterator != ':')
            ++currIterator;
    _args = ft::String::split(std::string(it, currIterator), " ");
    if (*currIterator == ':') {
        ++currIterator;
        if (currIterator != copy.end())
            _args.push_back(std::string(currIterator, copy.end()));
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
    if (cmd.getArgs().empty()) {
        os << "NaN";
        return os;
    }
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