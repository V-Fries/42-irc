#include "ft_String.hpp"

#include <cctype>

void    ft::String::toLower() {
    return toLower(this->begin(), this->end());
}

ft::String  ft::String::copyToLower() const {
    String  copy(*this);
    copy.toLower();
    return copy;
}

void    ft::String::toLower(iterator start,
                            const const_iterator end) {
    for (; start != end; ++start) {
        *start = static_cast<char>(std::tolower(static_cast<unsigned char>(*start)));
    }
}


void    ft::String::toUpper() {
    return toUpper(this->begin(), this->end());
}

ft::String  ft::String::copyToUpper() const {
    String  copy(*this);
    copy.toUpper();
    return copy;
}

void    ft::String::toUpper(iterator start,
                            const const_iterator end) {
    for (; start != end; ++start) {
        *start = static_cast<char>(std::toupper(static_cast<unsigned char>(*start)));
    }
}
