#include "String.hpp"

#include <iostream>

int	main() {
	std::vector<std::string> strings = ft::String::split("sepseptesetsepseptsepsep", "sep");

	for (std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); ++it) {
		std::cout << *it << std::endl;
	}

	std::cout << ft::String::join(strings) << std::endl;
}
