#include "ft_String.hpp"

#include <cctype>

void    ft::String::toLower() {
    map(&std::tolower);
}

ft::String  ft::String::copyToLower() const {
    return copyAndMap(&std::tolower);
}

void    ft::String::toLower(iterator start,
                            const const_iterator end) {
    map(start, end, &std::tolower);
}


void    ft::String::toUpper() {
    map(&std::toupper);
}

ft::String  ft::String::copyToUpper() const {
    return copyAndMap(&std::toupper);
}

void    ft::String::toUpper(iterator start,
                            const const_iterator end) {
    map(start, end, &std::toupper);
}
