#ifndef MY_IRC_SERVER_HPP
#define MY_IRC_SERVER_HPP

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sys/select.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include "Addition.hpp"
#include "Logger.hpp"


#define MAX_USERS 		15
#define POLL_TIMEOUT 	40000
#define TIMEOUT 		180

class Server {
public:
	Server(int port, int sql_port);
	void 						createFdList(int);

    int processConnection(int listen_sock);
	int 						createListenSocket(int);
	void						mainLoop(int);

	int 						getPort() const;

	Server 						&operator=(const Server &other);
	Server();
	~Server();
private:
	struct pollfd 				fd_list[1024];
	int							_port;
    int                         _sql_port;
	std::string					_serverName;

	Logger						logger;

    void						StartLogMessage();

};
#endif //MY_IRC_SERVER_HPP