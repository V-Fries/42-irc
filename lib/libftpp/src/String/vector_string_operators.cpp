#include "ft_String.hpp"

std::ostream&    operator<<(std::ostream& os, const std::vector<std::string>& strings) {
    if (strings.size() == 0) {
        os << "{}";
        return os;
    }

    const std::vector<std::string>::const_iterator  lastString = strings.end() - 1;
    std::vector<std::string>::const_iterator        it;
    os << '{';
    for (it = strings.begin(); it != lastString; ++it) {
        os << '"' << *it << "\", ";
    }
    os << '"' << *it << '"' << '}';
    return os;
}

std::vector<std::string>    operator+(const std::vector<std::string>& leftValue,
                                      const std::vector<std::string>& rightValue) {
    std::vector<std::string>    result(leftValue);

    for (std::vector<std::string>::const_iterator it = rightValue.begin();
         it != rightValue.end();
         ++it) {
        result.push_back(*it);
    }
    return result;
}

std::vector<std::string>&    operator+=(std::vector<std::string>& leftValue,
                                        const std::vector<std::string>& rightValue) {
    const std::vector<std::string>* vectorToAdd;
    std::vector<std::string>        tmp;
    if (&leftValue == &rightValue) {
        tmp = rightValue;
        vectorToAdd = &tmp;
    } else {
        vectorToAdd = &rightValue;
    }

    for (std::vector<std::string>::const_iterator it = vectorToAdd->begin();
         it != vectorToAdd->end();
         ++it) {
        leftValue.push_back(*it);
    }
    return leftValue;
}
