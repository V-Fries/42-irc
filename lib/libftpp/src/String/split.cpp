#include "ft_String.hpp"

static std::vector<std::string>    splitOnDelimiter(const std::string& string,
                                                 const std::string& delimiter);
static std::vector<std::string>    splitOnCharacterSet(const std::string& string,
                                                       const std::string& set);
//static std::vector<std::string>    splitUnlessInQuotes(const std::string& string,
//                                                       const std::string& delimiter);

std::vector<std::string> ft::String::split(const std::string& string,
                                           const std::string& delimiter,
                                           const uint8_t flags)
                                           throw(std::invalid_argument) {
    switch (flags) {
        case 0:
            return splitOnDelimiter(string, delimiter);
        case SPLIT_ON_CHARACTER_SET:
            return splitOnCharacterSet(string, delimiter);
        default:
            throw std::invalid_argument("Unexpected value for flags in ft::String::split()");
    }
}

static std::vector<std::string>    splitOnDelimiter(const std::string& string,
                                                 const std::string& delimiter) {
    std::vector<std::string>    result;
    size_t                      postNextDelimiter;

    for (size_t i = ft::String::findFirstNotOfString(string, delimiter);
         i != std::string::npos;
         i = ft::String::findFirstNotOfString(string, delimiter, postNextDelimiter)) {
        const size_t    nextDelimiter = string.find(delimiter, i);
        if (nextDelimiter != std::string::npos) {
            result.push_back(string.substr(i, nextDelimiter - i));
        } else {
            result.push_back(string.substr(i));
            return result;
        }
        postNextDelimiter = nextDelimiter + delimiter.length();
    }

    return result;
}

static std::vector<std::string>    splitOnCharacterSet(const std::string& string,
                                                       const std::string& set) {
    std::vector<std::string>    result;
    size_t                      postNextDelimiter;

    for (size_t i = string.find_first_not_of(set);
         i != std::string::npos;
         i = string.find_first_not_of(set, postNextDelimiter)) {
        const size_t    nextDelimiter = string.find_first_of(set, i);
        if (nextDelimiter != std::string::npos) {
            result.push_back(string.substr(i, nextDelimiter - i));
        } else {
            result.push_back(string.substr(i));
            return result;
        }
        postNextDelimiter = nextDelimiter + set.length();
    }

    return result;
}

//
//static std::vector<std::string>    splitUnlessInQuotes(const std::string& string,
//                                                       const std::string& delimiter) {
//    std::vector<std::string>    result;
//
//    static_cast<void>(string);
//    static_cast<void>(delimiter);
//    return result;
//}
