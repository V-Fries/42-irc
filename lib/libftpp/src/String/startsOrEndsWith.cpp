#include "ft_String.hpp"

bool    ft::String::startsWith(const String& word) const {
    return startsWith(word, this->begin(), this->end());
}

bool    ft::String::startsWith(const String& word,
                               const_iterator start,
                               const const_iterator end) {
    const_iterator  word_it(word.begin());

    while (word_it != word.end() && start != end) {
        if (*word_it != *start)
            return false;
        ++start;
        ++word_it;
    }
    return word_it == word.end();
}

bool    ft::String::endsWith(const String& word) const {
    return endsWith(word, this->rbegin(), this->rend());
}

bool    ft::String::endsWith(const String& word,
                             const_reverse_iterator rbegin,
                             const const_reverse_iterator rend) {
    const_reverse_iterator  word_it(word.begin());

    while (word_it != word.rend() && rbegin != rend) {
        if (*word_it != *rbegin)
            return false;
        ++rbegin;
        ++word_it;
    }
    return word_it == word.rend();
}