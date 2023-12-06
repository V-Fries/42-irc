#include "ft_String.hpp"

static std::vector<ft::String>  splitOnDelimiter(const ft::String& string,
                                                 const ft::String& delimiter);
static std::vector<ft::String>  splitOnCharacterSet(const ft::String& string,
                                                    const ft::String& set);
//static std::vector<ft::String>    splitUnlessInQuotes(const ft::String& string,
//                                                       const ft::String& delimiter);

std::vector<ft::String> ft::String::split(const String& delimiter,
                                          const uint8_t flags) const
                                        throw(std::invalid_argument) {
    if (delimiter.empty()) {
        std::vector<ft::String> result;
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

static std::vector<ft::String>  splitOnDelimiter(const ft::String& string,
                                                 const ft::String& delimiter) {
    std::vector<ft::String> result;

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

static std::vector<ft::String>  splitOnCharacterSet(const ft::String& string,
                                                    const ft::String& set) {
    std::vector<ft::String> result;

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
//static std::vector<ft::String>    splitUnlessInQuotes(const ft::String& string,
//                                                       const ft::String& delimiter) {
//    std::vector<ft::String>    result;
//
//    static_cast<void>(string);
//    static_cast<void>(delimiter);
//    return result;
//}
