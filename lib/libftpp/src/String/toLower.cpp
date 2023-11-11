#include "ft_String.hpp"

#include <cctype>

std::string ft::String::toLower(const std::string& string) {
    std::string result(string);

    for (std::string::iterator it(result.begin()); it != result.end(); ++it) {
        *it = static_cast<char>(std::tolower(static_cast<unsigned char>(*it)));
    }
    return result;
}
