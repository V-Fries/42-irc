#include "ft_String.hpp"

std::string  ft::String::getFirstWord(const std::string& string,
                                      const std::string& charsConsideredAsWordSeparators) {
    std::string::const_iterator   it;

    for (it = string.begin();
         it != string.end()
         && charsConsideredAsWordSeparators.find(*it) == string.npos;
         ++it);
    if (it == string.end())
        return std::string("");
    return std::string(string.begin(), it);
}

std::string  ft::String::getFirstWord(const std::string& string,
                                      const char charConsideredAsWordSeparator) {
    std::string::const_iterator   it;

    for (it = string.begin();
         it != string.end()
         && *it != charConsideredAsWordSeparator;
         ++it);
    if (it == string.end() || it == string.begin()) // TODO test if it == string.begin() is required
        return std::string("");
    return std::string(string.begin(), it);
}

