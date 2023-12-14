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

    void    remove(const T& value);
};