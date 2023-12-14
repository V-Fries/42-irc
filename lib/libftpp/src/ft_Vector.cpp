#include "ft_Vector.hpp"

template <typename T>
void    ft::Vector<T>::remove(const T& value) {
    size_t  i = 0;

    while (i < this->size()) {
        if (this->at(i) == value) {
            this->erase(this->begin() + i);
        } else {
            ++i;
        }
    }
}
