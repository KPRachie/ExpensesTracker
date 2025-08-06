#pragma once

#include <string>
#include <utility>
#include <vector>


namespace lib{
	inline std::vector<std::string> split(std::string string) {
		int64_t start = 0;
		std::vector<std::string> splitted_string;
		for (int64_t i = 0; i < string.size(); i++) {
			if (string[i] == ',') {
				splitted_string.emplace_back(string.begin() + start, string.begin() + i);
				start = i + 2;
			}
		}
		splitted_string.emplace_back(string.begin() + start, string.end());
		return splitted_string;
	}

	inline std::pair<std::string, std::string> tokenize(std::string string) {
		std::pair<std::string, std::string> token;
		for (int i = string.size() - 1; i > 0; i--) {
			if (!std::isdigit(string[i])) {
				token = std::make_pair(std::string(string.begin(), string.begin() + i), std::string(string.begin() + i + 1, string.end()));
				return token;
			}
		}
		return token;
	}
}
