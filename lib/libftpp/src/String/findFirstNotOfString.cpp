#include "ft_String.hpp"

size_t   ft::String::findFirstNotOfString(const String& toFind,
                                          size_t i) const {
    for (; i < this->length(); ++i) {
        size_t  j = 0;
        for (; j < toFind.length() && (*this)[i + j] == toFind[j]; ++j) {};
        if (j != toFind.length())
            return i;
    }
    return npos;
}
