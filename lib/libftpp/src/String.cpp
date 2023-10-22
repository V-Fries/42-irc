#include "String.hpp"


#include <iostream>
namespace ft {
	std::string	String::join(const std::vector<std::string>& strings) {
		std::string	result;

		for (std::vector<std::string>::const_iterator it = strings.begin(); it != strings.end(); ++it) {
			result += *it;
		}
		return result;
	}

	std::vector<std::string>	String::split(const std::string& string, const std::string& delimiter) {
		std::vector<std::string>	result;
		size_t						postNextDelimiter;


		for (size_t i = string.find_first_not_of(delimiter);
			i != std::string::npos;
			i = string.find_first_not_of(delimiter, postNextDelimiter)) {
			const size_t	nextDelimiter = string.find(delimiter, i);
			if (nextDelimiter != std::string::npos) {
				result.push_back(string.substr(i, nextDelimiter - i));
			} else {
				result.push_back(string.substr(i));
				return result;
			}
			postNextDelimiter = nextDelimiter + delimiter.length();
		}

		return result;
	}
};
