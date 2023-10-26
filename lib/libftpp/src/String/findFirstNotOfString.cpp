#include "ft_String.hpp"

size_t   ft::String::findFirstNotOfString(const std::string& string,
                                          const std::string& toFind,
                                          size_t i) {
    for (; i < string.length(); ++i) {
        size_t  j = 0;
        for (; j < toFind.length() && string[i + j] == toFind[j]; ++j);
        if (j != toFind.length())
            return i;
    }
    return std::string::npos;
}
