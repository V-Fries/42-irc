#pragma once

#include <iostream>

# define ANSI_BLACK "\e[0;30m"
# define ANSI_BLACK_HIGH_INTENSITY "\e[0;90m"

# define ANSI_GREEN "\e[0;32m"

# define ANSI_YELLOW "\e[0;33m"
# define ANSI_YELLOW_HIGH_INTENSITY "\e[1;33m"

# define ANSI_RED "\e[0;31m"


# define ANSI_BLUE "\e[0;34m"
# define ANSI_BLUE_HIGH_INTENSITY "\e[0;94m"

# define ANSI_PURPLE "\e[0;35m"
# define ANSI_PURPLE_BOLD "\e[1;35m"
# define ANSI_PURPLE_BOLD_HIGH_INTENSITY "\e[1;95m"

# define ANSI_CYAN "\e[0;36m"
# define ANSI_CYAN_HIGH_INTENSITY "\e[0;96m"

# define ANSI_WHITE "\e[0;37m"

# define ANSI_RESET_COLOR "\e[m"

namespace ft {
    class Io;
};

class ft::Io {
    public:
        static std::ostream&   green(std::ostream& os);
        static std::ostream&   yellow(std::ostream& os);
        static std::ostream&   orange(std::ostream& os);
        static std::ostream&   red(std::ostream& os);
        static std::ostream&   resetColor(std::ostream& os);
};
