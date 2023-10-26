#include <iostream>

#include "ft.hpp"

#include "poll.h"
int main(int argc, char** argv) {
    static_cast<void>(argc);
    static_cast<void>(argv);
    std::vector<std::string> test = ft::String::split("eT  Test  ", "T ", SPLIT_ON_CHARACTER_SET);

    std::cout << test << std::endl << std::endl;
    test = ft::String::split("eT  Test  ", "T ");
    std::cout << test << std::endl;
}
