#include "ft_String.hpp"

static ft::Vector<ft::String>  splitOnDelimiter(const ft::String& string,
                                                 const ft::String& delimiter);
static ft::Vector<ft::String>  splitOnCharacterSet(const ft::String& string,
                                                    const ft::String& set);
//static ft::Vector<ft::String>    splitUnlessInQuotes(const ft::String& string,
//                                                       const ft::String& delimiter);

ft::Vector<ft::String> ft::String::split(const String& delimiter,
                                          const uint8_t flags) const
                                        throw(std::invalid_argument) {
    if (delimiter.empty()) {
        ft::Vector<ft::String> result;
        result.push_back(*this);
        return result;
    }

    switch (flags) {
        case 0:
            return splitOnDelimiter(*this, delimiter);
        case SPLIT_ON_CHARACTER_SET:
            return splitOnCharacterSet(*this, delimiter);
        default:
            throw std::invalid_argument("Unexpected value for flags in ft::String::split()");
    }
}

static ft::Vector<ft::String>  splitOnDelimiter(const ft::String& string,
                                                 const ft::String& delimiter) {
    ft::Vector<ft::String> result;

    size_t  postNextDelimiter;
    for (size_t i = string.findFirstNotOfString(delimiter);
         i != ft::String::npos;
         i = string.findFirstNotOfString(delimiter, postNextDelimiter)) {
        const size_t    nextDelimiter = string.find(delimiter, i);
        if (nextDelimiter == ft::String::npos) {
            result.push_back(string.substr(i));
            return result;
        }
        result.push_back(string.substr(i, nextDelimiter - i));
        postNextDelimiter = nextDelimiter + delimiter.length();
    }

    return result;
}

static ft::Vector<ft::String>  splitOnCharacterSet(const ft::String& string,
                                                    const ft::String& set) {
    ft::Vector<ft::String> result;

    size_t  postNextDelimiter;
    for (size_t i = string.find_first_not_of(set);
         i != ft::String::npos;
         i = string.find_first_not_of(set, postNextDelimiter)) {
        const size_t    nextDelimiter = string.find_first_of(set, i);
        if (nextDelimiter == ft::String::npos) {
            result.push_back(string.substr(i));
            return result;
        }
        result.push_back(string.substr(i, nextDelimiter - i));
        postNextDelimiter = nextDelimiter + 1;
    }

    return result;
}

//
//static ft::Vector<ft::String>    splitUnlessInQuotes(const ft::String& string,
//                                                       const ft::String& delimiter) {
//    ft::Vector<ft::String>    result;
//
//    static_cast<void>(string);
//    static_cast<void>(delimiter);
//    return result;
//}
