#include "ft_String.hpp"

#include <cctype>

std::string ft::String::toLower(const std::string& string) {
    return toLower(string.begin(), string.end());
}

std::string ft::String::toLower(const std::string::const_iterator start,
                                const std::string::const_iterator end) {
    std::string result(start, end);

    for (std::string::iterator it(result.begin()); it != result.end(); ++it) {
        *it = static_cast<char>(std::tolower(static_cast<unsigned char>(*it)));
    }
    return result;
}


std::string ft::String::toUpper(const std::string& string) {
    return toUpper(string.begin(), string.end());
}

std::string ft::String::toUpper(const std::string::const_iterator start,
                                const std::string::const_iterator end) {
    std::string result(start, end);

    for (std::string::iterator it(result.begin()); it != result.end(); ++it) {
        *it = static_cast<char>(std::toupper(static_cast<unsigned char>(*it)));
    }
    return result;
}
