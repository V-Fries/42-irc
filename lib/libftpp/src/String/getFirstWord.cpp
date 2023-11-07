#include "ft_String.hpp"

std::string  ft::String::getFirstWord(const std::string& string,
                                      const std::string& charsConsideredAsWordSeparators) {
    return ft::String::getFirstWord(string.begin(), string.end(),
                                    charsConsideredAsWordSeparators);
}

std::string  ft::String::getFirstWord(const std::string& string,
                                      const char charConsideredAsWordSeparator) {
    return ft::String::getFirstWord(string.begin(), string.end(),
                                    charConsideredAsWordSeparator);
}

std::string  ft::String::getFirstWord(std::string::const_iterator it,
                                      std::string::const_iterator end,
                                      const std::string& charsConsideredAsWordSeparators) {
    std::string::const_iterator firstChar = it;
    while (firstChar != end
           && charsConsideredAsWordSeparators.find(*firstChar) != std::string::npos)
        ++firstChar;

    std::string::const_iterator lastChar = firstChar;
    while (lastChar != end
           && charsConsideredAsWordSeparators.find(*lastChar) == std::string::npos)
        ++lastChar;

    return std::string(firstChar, lastChar);
}

std::string  ft::String::getFirstWord(std::string::const_iterator it,
                                      std::string::const_iterator end,
                                      const char charConsideredAsWordSeparator) {
    std::string::const_iterator firstChar = it;
    while (firstChar != end && *firstChar == charConsideredAsWordSeparator)
        ++firstChar;

    std::string::const_iterator lastChar = firstChar;
    while (lastChar != end && *lastChar != charConsideredAsWordSeparator)
        ++lastChar;

    return std::string(firstChar, lastChar);
}
