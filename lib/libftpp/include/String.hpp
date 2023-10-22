#pragma once

#include <string>
#include <vector>

namespace ft {
	class String {
	public:
		static std::string	join(const std::vector<std::string>& strings);

		static std::vector<std::string>	split(const std::string& string, const std::string& delimiter);
	};
};
