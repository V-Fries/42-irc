#include "test_string.hpp"
#include "ft_Io.hpp"
#include <iostream>

int main() {
    std::cerr << ft::Io::green << "Testing split:" << ft::Io::red << std::endl;
    test_split();

    std::cerr << ft::Io::green << "Tests done" << ft::Io::resetColor << std::endl;
}
