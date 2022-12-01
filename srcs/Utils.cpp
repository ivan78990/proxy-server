#include <string>
#include <vector>
#include "../includes/Addition.hpp"

std::string	currentTime() {
	time_t curr_time;
	tm * curr_tm;
	char date_string[100];

	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(date_string, 50, "%Y-%m-%d %T", curr_tm);
	return date_string;
}

