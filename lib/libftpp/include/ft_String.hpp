#pragma once

#include "ft_Vector.hpp"
#include <ostream>
#include <stdexcept>
#include <stdint.h>

#define SPLIT_ON_CHARACTER_SET (1 << 0)
#define SPLIT_UNLESS_IN_QUOTES (1 << 1)

namespace ft {
    class String;
};

class ft::String : public std::string {
    public:
        String():
            std::string() {}

        String(const String& str):
            std::string(str) {}

        String(const std::string& str):
            std::string(str) {}

        String(const String& str, size_t pos, size_t len = npos):
            std::string(str, pos, len) {}

        String(const std::string& str, size_t pos, size_t len = npos):
            std::string(str, pos, len) {}

        String(const char* s):
            std::string(s) {}

        String(const char* s, const size_t n):
            std::string(s, n) {}

        String(size_t n, char c):
            std::string(n, c) {}

        template <class InputIterator>
        String(InputIterator first, InputIterator last):
            std::string(first, last) {}

        static String   join(const ft::Vector<String>& strings,
                             const String& separator);

        ///
        /// \param delimiter
        /// \param flags (SPLIT_ON_CHARACTER_SET | SPLIT_UNLESS_IN_QUOTES)
        ///              will split on each chars of the delimiter ignoring the chars
        ///              that are between quotes
        /// \return
        ft::Vector<String> split(const String& delimiter,
                                  uint8_t flags = 0) const
                                throw(std::invalid_argument);

        /// \brief "4242a".findFirstNotOfString("42") will return the index of 'a'
        /// not the index of the first '2'
        /// \param toFind string to look for
        /// \param pos position to start at
        /// \return index of the first char that isn't part of toFind
        size_t   findFirstNotOfString(const String& toFind, size_t pos = 0) const;

        static iterator         skipChars(iterator it,
                                          iterator end,
                                          const String& charsToSkip);
        static const_iterator   skipChars(const_iterator it,
                                          const_iterator end,
                                          const String& charsToSkip);
        static iterator         skipChar(iterator it,
                                         iterator end,
                                         char charToSkip);
        static const_iterator   skipChar(const_iterator it,
                                         const_iterator end,
                                         char charToSkip);

        String          getFirstWord(const String& charsConsideredAsWordSeparators) const;
        String          getFirstWord(char charConsideredAsWordSeparator) const;
        static String   getFirstWord(const_iterator it,
                                     const_iterator end,
                                     const String& charsConsideredAsWordSeparators);
        static String   getFirstWord(const_iterator it,
                                     const_iterator end,
                                     char charConsideredAsWordSeparator);

        bool        startsWith(const String& word) const;
        static bool startsWith(const String& word,
                               const_iterator start,
                               const_iterator end);
        bool        endsWith(const String& word) const;
        static bool endsWith(const String& word,
                             const_reverse_iterator rbegin,
                             const_reverse_iterator rend);

        void        map(int (*fn)(int));
        String      copyAndMap(int (*fn)(int)) const;
        static void map(iterator start, const_iterator end, int (*fn)(int));

        void        toLower();
        String      copyToLower() const;
        static void toLower(iterator start, const_iterator end);
        void        toUpper();
        String      copyToUpper() const;
        static void toUpper(iterator start, const_iterator end);
};

std::ostream&   operator<<(std::ostream& os, const ft::Vector<ft::String>& strings);

ft::Vector<ft::String> operator+(const ft::Vector<ft::String>& leftValue,
                                  const ft::Vector<ft::String>& rightValue);

ft::Vector<ft::String>&    operator+=(ft::Vector<ft::String>& leftValue,
                                       const ft::Vector<ft::String>& rightValue);
