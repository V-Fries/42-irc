#include "ft_String.hpp"

void    ft::String::map(int (*fn)(int)) {
    map(this->begin(), this->end(), fn);
}

ft::String  ft::String::copyAndMap(int (*fn)(int)) const {
    String copy(*this);
    copy.map(fn);
    return copy;
}

void    ft::String::map(iterator start, const_iterator end, int (*fn)(int)) {
    for (; start != end; ++start) {
        *start = static_cast<char>(fn(static_cast<unsigned char>(*start)));
    }
}
