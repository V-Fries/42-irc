#ifndef COMMAND_HPP
# define COMMAND_HPP

# include <string>
# include <vector>

class Command {
    public:
        explicit Command(const std::string &rawCommand);

        const std::string               &getCommand() const;
        const std::vector<std::string>  &getArgs() const;
    private:
        std::string                 _command;
        std::vector<std::string>    _args;
};

std::ostream& operator<<(std::ostream& os, const Command& cmd);

#endif
