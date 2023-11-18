#include "ft_String.hpp"

std::string::iterator    ft::String::skipChars(std::string::iterator it,
                                               const std::string::iterator end,
                                               const std::string& charsToSkip) {
    while (it != end && charsToSkip.find(*it) != std::string::npos) {
        ++it;
    }
    return it;
}

std::string::const_iterator  ft::String::skipChars(std::string::const_iterator it,
                                                   const std::string::const_iterator end,
                                                   const std::string& charsToSkip) {
    while (it != end && charsToSkip.find(*it) != std::string::npos) {
        ++it;
    }
    return it;
}

std::string::iterator    ft::String::skipChar(std::string::iterator it,
                                              const std::string::iterator end,
                                              const char charToSkip) {
    while (it != end && *it == charToSkip) {
        ++it;
    }
    return it;
}

std::string::const_iterator  ft::String::skipChar(std::string::const_iterator it,
                                                  const std::string::const_iterator end,
                                                  const char charToSkip) {
    while (it != end && *it == charToSkip) {
        ++it;
    }
    return it;
}
