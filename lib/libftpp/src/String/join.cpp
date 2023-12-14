#include "ft_String.hpp"

ft::String ft::String::join(const ft::Vector<String>& strings,
                             const String& separator) {
    if (strings.size() == 0) return String("");

    String                                      result;
    const ft::Vector<String>::const_iterator    lastString = strings.end() - 1;
    ft::Vector<String>::const_iterator          it;
    for (it = strings.begin(); it != lastString; ++it) {
        result += *it + separator;
    }
    return result + *it;
}
