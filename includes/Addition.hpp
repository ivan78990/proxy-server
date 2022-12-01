#ifndef MY_IRC_MY_IRC_HPP
#define MY_IRC_MY_IRC_HPP

#define CYAN "\033[36m"
#define RESET "\033[0m"

#define MORE_INFO
#define LOGGER_PATH "log"



class FtException: public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Look for FtException";
	}
};

std::string					getmessage(std::string &);

std::string					currentTime();

#endif //MY_IRC_MY_IRC_HPP