#include <iostream>
#include "../includes/Addition.hpp"
#include "../includes/Server.hpp"
#include <stdlib.h>

void validateArguments(int ac, char **av) {
	if (ac < 3) {
		std::cout << "Too low _arguments!" << std::endl;
		std::cout << "Usage: ./proxy listen<port> sql-server<port>" << std::endl;
		std::cout << "Example: ./proxy 6667 5432" << std::endl;
		exit(-1);
	}
	if (ac > 3) {
        std::cout << "Usage: ./proxy listen<port> sql-server<port>" << std::endl;
        std::cerr << "Too many _arguments!" << std::endl;
        exit(-1);
    }

	try {
		if ((std::stoi(av[1])) < 0)
			throw FtException();
	}
	catch (std::exception &e) {
		std::cerr << "invalid argument: port" << std::endl;
#ifdef MORE_INFO
		std::cerr << CYAN << e.what() << RESET << std::endl;
#endif
		exit(-1);
	}
}

int main(int ac, char **av)
{
	validateArguments(ac, av);
	try {
		signal(SIGPIPE, SIG_IGN);
		Server server = Server(std::stoi(av[1]), std::stoi(av[2]));
		int listen_sock = server.createListenSocket(server.getPort());
		server.mainLoop(listen_sock);
	}
	catch (std::exception &e) {
		std::cerr << "An error occurred" << std::endl;
		#ifdef MORE_INFO
		std::cerr << CYAN << e.what() << RESET << std::endl;
		#endif
		exit(-1);
	}
	return 0;
}