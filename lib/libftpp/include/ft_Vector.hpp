#pragma once

#include <vector>
#include <cstddef>

namespace ft {
    template <typename T>
    class Vector;
};

template <typename T>
class ft::Vector : public std::vector<T> {
    public:

    Vector():
        std::vector<T>() {}

    template <class InputIterator>
    Vector(InputIterator first, InputIterator last):
        std::vector<T>(first, last) {}

    void    remove(const T& value) {
        size_t  i = 0;

        while (i < this->size()) {
            if (this->at(i) == value) {
                this->erase(this->begin() + i);
            } else {
                ++i;
            }
        }
    }

    T   join(const T& separator) {
        if (this->size() == 0) return T();

        typename Vector::const_iterator  it = this->begin();
        T                                result = *it;

        ++it;
        for (; it < this->end(); ++it) {
            result += separator + *it;
        }
        return result;
    }
};