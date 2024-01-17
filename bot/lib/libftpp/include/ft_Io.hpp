#pragma once

#include <iostream>

# define ANSI_BLACK "\001\e[0;30m\002"
# define ANSI_BLACK_HIGH_INTENSITY "\001\e[0;90m\002"

# define ANSI_GREEN "\001\e[0;32m\002"

# define ANSI_YELLOW "\001\e[0;33m\002"
# define ANSI_YELLOW_HIGH_INTENSITY "\001\e[1;33m\002"

# define ANSI_RED "\001\e[0;31m\002"


# define ANSI_BLUE "\001\e[0;34m\002"
# define ANSI_BLUE_HIGH_INTENSITY "\001\e[0;94m\002"

# define ANSI_PURPLE "\001\e[0;35m\002"
# define ANSI_PURPLE_BOLD "\001\e[1;35m\002"
# define ANSI_PURPLE_BOLD_HIGH_INTENSITY "\001\e[1;95m\002"

# define ANSI_CYAN "\001\e[0;36m\002"
# define ANSI_CYAN_HIGH_INTENSITY "\001\e[0;96m\002"

# define ANSI_WHITE "\001\e[0;37m\002"

# define ANSI_RESET_COLOR "\001\e[m\002"

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
