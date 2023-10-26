#include <iostream>

#include "ft.hpp"

#include "poll.h"
int main(int argc, char** argv) {
    static_cast<void>(argc);static_cast<void>(argv);

    std::vector<struct pollfd> vector;

    vector.push_back((struct pollfd){0, 0, 0});

    struct pollfd*  pollfd = &vector[0];

    if (pollfd == &vector.front()) {
        return 0;
    }
    return 1;
}
