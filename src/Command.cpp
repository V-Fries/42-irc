#include "Command.hpp"
#include "ft.hpp"

Command::Command(const std::string &rawCommand) {
    std::string::const_iterator it = rawCommand.begin();
    std::string::const_iterator currIterator;

    if (rawCommand[0] == ':') {
        while (!isspace(*it))
            ++it;
        while (isspace(*it))
            ++it;
    }
    for (   currIterator = it ; \
            currIterator != rawCommand.end() && \
            isupper(*currIterator) && \
            !isspace(*currIterator); \
            ++currIterator) { }
    _command = std::string(it, currIterator);
    it = currIterator;
    for (   currIterator = it ; \
            currIterator != rawCommand.end() && \
            *currIterator != ':'; \
            ++currIterator) { }
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
    os << "Command: " << cmd.getCommand() << '\n';
    os << "Args: " << '\n';
    for (std::vector<std::string>::const_iterator it = cmd.getArgs().begin(); it != cmd.getArgs().end(); ++it) {
        os << *it << '\n';
    }
    os << std::flush;
    return os;
}