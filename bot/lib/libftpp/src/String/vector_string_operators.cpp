#include "ft_String.hpp"

std::ostream&    operator<<(std::ostream& os, const std::vector<ft::String>& strings) {
    if (strings.size() == 0) {
        os << "{}";
        return os;
    }

    const std::vector<ft::String>::const_iterator  lastString = strings.end() - 1;
    std::vector<ft::String>::const_iterator        it;
    os << '{';
    for (it = strings.begin(); it != lastString; ++it) {
        os << '"' << *it << "\", ";
    }
    os << '"' << *it << '"' << '}';
    return os;
}

std::vector<ft::String> operator+(const std::vector<ft::String>& leftValue,
                                  const std::vector<ft::String>& rightValue) {
    std::vector<ft::String>    result(leftValue);

    for (std::vector<ft::String>::const_iterator it = rightValue.begin();
         it != rightValue.end();
         ++it) {
        result.push_back(*it);
    }
    return result;
}

std::vector<ft::String>&    operator+=(std::vector<ft::String>& leftValue,
                                       const std::vector<ft::String>& rightValue) {
    const std::vector<ft::String>* vectorToAdd;
    std::vector<ft::String>        tmp;
    if (&leftValue == &rightValue) {
        tmp = rightValue;
        vectorToAdd = &tmp;
    } else {
        vectorToAdd = &rightValue;
    }

    for (std::vector<ft::String>::const_iterator it = vectorToAdd->begin();
         it != vectorToAdd->end();
         ++it) {
        leftValue.push_back(*it);
    }
    return leftValue;
}
