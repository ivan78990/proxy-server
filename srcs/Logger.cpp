#include "../includes/Logger.hpp"

Logger::~Logger() {
}

Logger::Logger() {
	filename = LOGGER_PATH;
	file.open(filename, MODE);
	if (!file )
	{
		//Cannot open file, file does not exist. Creating new file
		file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
		file << "\n";
	}
	file.close();
}

void Logger::logMessage(std::string message, LogType type) {
	std::stringstream logStream;
	std::stringstream lol(message);
	std::string			line;

	while (std::getline(lol, line)){
		if (line.size() > 2 && line.at(line.size() - 1) == '\r')
			line = line.substr(0, line.size() - 1);
		logStream << line << std::endl;
	}
	logMessage(logStream, type);
}

void Logger::logMessage(std::stringstream &message, LogType type) {
	std::stringstream out;
	std::string	line;
	std::string	typeString;

	while (std::getline(message, line)){
		out << currentTime();
		switch (type) {
			case IN:
				out << "|<<<<<|";
				break;
			case OUT:
				out << "|>>>>>|";
				break;
			case INFO:
				out << "|INFO |";
				break;
			case ERROR:
				out << "|ERROR|";
				break;
			case DEV:
				out << "|DEV  |";
				break;
		}
		out << line << std::endl;
	}
	std::cout << out.str();
	file.open(filename, MODE);
	file << out.str();
	file.close();
	message.clear();
}

Logger &Logger::operator=(const Logger &) {
	return *this;
}

Logger::Logger(const Logger &other) {
	*this = other;
}

