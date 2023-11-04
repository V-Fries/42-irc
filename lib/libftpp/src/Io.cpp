#include "ft_Io.hpp"

std::ostream&   ft::Io::green(std::ostream& os) {
    os << ANSI_GREEN;
    return os;
}

std::ostream&   ft::Io::yellow(std::ostream& os) {
    os << ANSI_YELLOW_HIGH_INTENSITY;
    return os;
}

std::ostream&   ft::Io::orange(std::ostream& os) {
    os << ANSI_PURPLE;
    return os;
}

std::ostream&   ft::Io::red(std::ostream& os) {
    os << ANSI_RED;
    return os;
}

std::ostream&   ft::Io::resetColor(std::ostream& os) {
    os << ANSI_RESET_COLOR << std::flush;
    return os;
}
