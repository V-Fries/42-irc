#pragma once

#include <set>

namespace ft {
    template <typename T>
    class Set;
};

template <typename Key>
class ft::Set : public std::set<Key> {
    public:
        bool    contains(const Key& key) const {
            return this->find(key) != this->end();
        }
};
