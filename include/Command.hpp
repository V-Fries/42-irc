#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "ft_String.hpp"

# include <vector>

class Command {
    public:
        explicit Command(const ft::String &rawCommand);

        const ft::String               &getCommand() const;
        const std::vector<ft::String>  &getArgs() const;
    private:
        ft::String                 _command;
        std::vector<ft::String>    _args;
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif
