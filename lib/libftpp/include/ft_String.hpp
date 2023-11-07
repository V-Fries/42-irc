#pragma once

#include <ostream>
#include <vector>
#include <stdexcept>
#include <stdint.h>

#define SPLIT_ON_CHARACTER_SET (1 << 0)
#define SPLIT_UNLESS_IN_QUOTES (1 << 1)

namespace ft {
    class String;
};

class ft::String {
    public:
        static std::string  join(const std::vector<std::string>& strings,
                                 const std::string& separator);

        ///
        /// \param string string to split
        /// \param delimiter
        /// \param flags (SPLIT_ON_CHARACTER_SET | SPLIT_UNLESS_IN_QUOTES)
        ///              will split on each chars of the delimiter ignoring the chars
        ///              that are between quotes
        /// \return
        static std::vector<std::string> split(const std::string& string,
                                              const std::string& delimiter,
                                              const uint8_t flags = 0)
                                              throw(std::invalid_argument);

        static size_t   findFirstNotOfString(const std::string& string,
                                             const std::string& toFind,
                                             size_t i = 0);

        static std::string::iterator        skipChars(std::string::iterator it,
                                                      std::string::iterator end,
                                                      const std::string& charsToSkip);
        static std::string::const_iterator  skipChars(std::string::const_iterator it,
                                                      std::string::const_iterator end,
                                                      const std::string& charsToSkip);
        static std::string::iterator        skipChar(std::string::iterator it,
                                                     std::string::iterator end,
                                                     char charToSkip);
        static std::string::const_iterator  skipChar(std::string::const_iterator it,
                                                     std::string::const_iterator end,
                                                     char charToSkip);

        static std::string  getFirstWord(const std::string& string,
                                         const std::string& charsConsideredAsWordSeparators);
        static std::string  getFirstWord(const std::string& string,
                                         char charConsideredAsWordSeparator);
        static std::string  getFirstWord(std::string::const_iterator it,
                                         std::string::const_iterator end,
                                         const std::string& charsConsideredAsWordSeparators);
        static std::string  getFirstWord(std::string::const_iterator it,
                                         std::string::const_iterator end,
                                         char charConsideredAsWordSeparator);

};

std::ostream&   operator<<(std::ostream& os, std::vector<std::string> strings);

std::vector<std::string>    operator+(const std::vector<std::string>& leftValue,
                                      const std::vector<std::string>& rightValue);

std::vector<std::string>&    operator+=(std::vector<std::string>& leftValue,
                                        const std::vector<std::string>& rightValue);
