#include <string>
#include <vector>
#include "../includes/Addition.hpp"

bool	allowedCharacter(char c) {
	if ((c >= 65 && c <= 90) /*'A'-'Z'*/ || (c >= 97 && c <= 123) /*'a'-'z'*/ || (c >= 48 && c <= 57) /*'0'-'9'*/ || (c == '-' || c == '_'))
		return true;
	return false;
}

bool validateString(const std::string &string) {
	for (std::vector<const char>::iterator it = string.begin(); it != string.end(); ++it)
		if (!allowedCharacter(*it))
			return false;
	return true;
}

std::string	trim(const std::string& string) {
	const char*	whitespace = " \t\n\r\f\v";
	size_t begin = string.find_first_not_of(whitespace);
	if (begin == std::string::npos) {
		return std::string();
	}
	size_t end = string.find_last_not_of(whitespace);
	return std::string(string.substr(begin, end - begin + 1));
}

std::string toLowercase(std::string const &original) {
	std::string lowercased;

	for (std::vector<char const>::iterator it = original.begin(); it != original.end(); ++it) {
		lowercased += std::tolower(*it);
	}
	return lowercased;
}

std::string toUppercase(std::string const &original) {
	std::string uppercased;

	for (std::vector<char const>::iterator it = original.begin(); it != original.end(); ++it) {
		uppercased += std::toupper(*it);
	}
	return uppercased;
}

std::string	currentTime() {
	time_t curr_time;
	tm * curr_tm;
	char date_string[100];

	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(date_string, 50, "%Y-%m-%d %T", curr_tm);
	return date_string;
}

