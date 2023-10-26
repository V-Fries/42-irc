#include "ft_String.hpp"

std::string ft::String::join(const std::vector<std::string>& strings,
                             const std::string& separator) {
    if (strings.size() == 0) return std::string("");

    std::string                                     result;
    const std::vector<std::string>::const_iterator  lastString = strings.end() - 1;
    std::vector<std::string>::const_iterator        it;
    for (it = strings.begin(); it != lastString; ++it) {
        result += *it + separator;
    }
    return result + *it;
}
