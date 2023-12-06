#include "ft_String.hpp"

ft::String  ft::String::getFirstWord(const String& charsConsideredAsWordSeparators) const {
    return getFirstWord(this->begin(), this->end(),
                        charsConsideredAsWordSeparators);
}

ft::String  ft::String::getFirstWord(const char charConsideredAsWordSeparator) const {
    return getFirstWord(this->begin(), this->end(),
                        charConsideredAsWordSeparator);
}

ft::String  ft::String::getFirstWord(const const_iterator it,
                                     const const_iterator end,
                                     const String& charsConsideredAsWordSeparators) {
    const_iterator firstChar = it;
    while (firstChar != end
           && charsConsideredAsWordSeparators.find(*firstChar) != npos)
        ++firstChar;

    const_iterator lastChar = firstChar;
    while (lastChar != end
           && charsConsideredAsWordSeparators.find(*lastChar) == npos)
        ++lastChar;

    return String(firstChar, lastChar);
}

ft::String  ft::String::getFirstWord(const const_iterator it,
                                     const const_iterator end,
                                     const char charConsideredAsWordSeparator) {
    const_iterator firstChar = it;
    while (firstChar != end && *firstChar == charConsideredAsWordSeparator)
        ++firstChar;

    const_iterator lastChar = firstChar;
    while (lastChar != end && *lastChar != charConsideredAsWordSeparator)
        ++lastChar;

    return String(firstChar, lastChar);
}
