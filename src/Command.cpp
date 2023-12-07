#include "Command.hpp"
#include "ft.hpp"

static ft::String               getCommand(const ft::String& rawCommand,
                                           ft::String::const_iterator& it);
static std::vector<ft::String>  getArgs(const ft::String& rawCommand,
                                        ft::String::const_iterator& it);

Command::Command(const ft::String &rawCommand) {
    ft::String::const_iterator  it = rawCommand.begin();

    _command = ::getCommand(rawCommand, it);

    _args = ::getArgs(rawCommand, it);
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

static ft::String getCommand(const ft::String& rawCommand,
                             ft::String::const_iterator& it) {
    while (it != rawCommand.end() && isspace(*it)) ++it;
    if (it != rawCommand.end() && *it == ':')
        while (it != rawCommand.end() && !isspace(*it)) ++it;
    while (it != rawCommand.end() && isspace(*it)) ++it;
    if (it == rawCommand.end()) {
        it = rawCommand.end();
        return "";
    }

    ft::String command = ft::String::getFirstWord(it,
                                                  rawCommand.end(),
                                                  ": ");
    it += command.length();
    return (command);
}

std::vector<ft::String> getArgs(const ft::String& rawCommand,
                                ft::String::const_iterator& it) {
    while (it != rawCommand.end() && isspace(*it)) ++it;
    if (it == rawCommand.end()) {
        return std::vector<ft::String>();
    }

    ft::String::const_iterator currIterator = it;
    while (currIterator != rawCommand.end() && \
            *currIterator != ':')
        ++currIterator;

    std::vector<ft::String> args = ft::String(it, currIterator).split(" ");

    if (currIterator != rawCommand.end() && *currIterator == ':') {
        ++currIterator;
        if (currIterator != rawCommand.end())
            args.push_back(ft::String(currIterator, rawCommand.end()));
    }
    return args;
}
