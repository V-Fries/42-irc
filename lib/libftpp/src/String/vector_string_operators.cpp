#include "ft_String.hpp"

std::ostream&    operator<<(std::ostream& os, const ft::Vector<ft::String>& strings) {
    if (strings.size() == 0) {
        os << "{}";
        return os;
    }

    const ft::Vector<ft::String>::const_iterator  lastString = strings.end() - 1;
    ft::Vector<ft::String>::const_iterator        it;
    os << '{';
    for (it = strings.begin(); it != lastString; ++it) {
        os << '"' << *it << "\", ";
    }
    os << '"' << *it << '"' << '}';
    return os;
}

ft::Vector<ft::String> operator+(const ft::Vector<ft::String>& leftValue,
                                  const ft::Vector<ft::String>& rightValue) {
    ft::Vector<ft::String>    result(leftValue);

    for (ft::Vector<ft::String>::const_iterator it = rightValue.begin();
         it != rightValue.end();
         ++it) {
        result.push_back(*it);
    }
    return result;
}

ft::Vector<ft::String>&    operator+=(ft::Vector<ft::String>& leftValue,
                                       const ft::Vector<ft::String>& rightValue) {
    const ft::Vector<ft::String>* vectorToAdd;
    ft::Vector<ft::String>        tmp;
    if (&leftValue == &rightValue) {
        tmp = rightValue;
        vectorToAdd = &tmp;
    } else {
        vectorToAdd = &rightValue;
    }

    for (ft::Vector<ft::String>::const_iterator it = vectorToAdd->begin();
         it != vectorToAdd->end();
         ++it) {
        leftValue.push_back(*it);
    }
    return leftValue;
}
