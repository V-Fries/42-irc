#include "ft_String.hpp"

size_t   ft::String::findFirstNotOfString(const String& toFind,
                                          const size_t pos) const {
    const_iterator  it(this->begin() + pos);

    while (it < this->end()) {
        if (!startsWith(toFind, it, this->end()))
            return it - this->begin();
        it += toFind.length();
    }
    return npos;
}
