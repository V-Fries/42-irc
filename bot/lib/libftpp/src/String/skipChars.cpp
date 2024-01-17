#include "ft_String.hpp"

ft::String::iterator    ft::String::skipChars(iterator it,
                                              const iterator end,
                                              const String& charsToSkip) {
    while (it != end && charsToSkip.find(*it) != npos) {
        ++it;
    }
    return it;
}

ft::String::const_iterator  ft::String::skipChars(const_iterator it,
                                                  const const_iterator end,
                                                  const String& charsToSkip) {
    while (it != end && charsToSkip.find(*it) != npos) {
        ++it;
    }
    return it;
}

ft::String::iterator    ft::String::skipChar(iterator it,
                                             const iterator end,
                                             const char charToSkip) {
    while (it != end && *it == charToSkip) {
        ++it;
    }
    return it;
}

ft::String::const_iterator  ft::String::skipChar(const_iterator it,
                                                 const const_iterator end,
                                                 const char charToSkip) {
    while (it != end && *it == charToSkip) {
        ++it;
    }
    return it;
}
