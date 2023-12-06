#include "ft_String.hpp"

ft::String ft::String::join(const std::vector<String>& strings,
                             const String& separator) {
    if (strings.size() == 0) return String("");

    String                                     result;
    const std::vector<String>::const_iterator  lastString = strings.end() - 1;
    std::vector<String>::const_iterator        it;
    for (it = strings.begin(); it != lastString; ++it) {
        result += *it + separator;
    }
    return result + *it;
}
