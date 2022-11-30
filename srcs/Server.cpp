#include "../includes/Server.hpp"

Server::Server(int port, int sql_port) : _port(port), _sql_port(sql_port), _serverName("Proxy_Server") {
}

Server::Server() : _port(-1), _sql_port(-1) {
}

int Server::createListenSocket(int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if( sock < 0 )
		logger.logMessage("Socket fail", ERROR);
	int opt = 1;
	if (fcntl(sock, F_SETFL, O_NONBLOCK)) {
		logger.logMessage("Could not set non-blocking socket", ERROR);
		exit(-1);
	}
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if( bind(sock,(struct sockaddr *)&local,sizeof(local)) < 0 ) {
		logger.logMessage("bind fail on port " + std::to_string(port), ERROR);
		exit(-1);
	}

	if( listen(sock,5) < 0 ) {
		logger.logMessage("listen fail on port " + std::to_string(port), ERROR);
		exit(-1);
	}
	return sock;
}

void Server::StartLogMessage() {
	std::stringstream logMessageStream;

	logMessageStream << "Starting server on port " << this->_port << ' ';
	logger.logMessage(logMessageStream, INFO);
}

void Server::createFdList(int listen_socket)
{
	int i = 0;

	for (; i < MAX_USERS; i++) {
		fd_list[i].fd = -1;// File descriptor
		fd_list[i].events = 0;// Set of events to monitor
		fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
	}
	i = 0;
	for (; i < MAX_USERS; i++) {
		if (fd_list[i].fd == -1)
		{
			fd_list[i].fd = listen_socket;
			fd_list[i].events = POLLIN;// Concern about Read-Only Events
			break;
		}
	}
}

void Server::mainLoop(int listen_sock)
{
	StartLogMessage();
	createFdList(listen_sock);


	while (1)
	{
        std::stringstream logStream;
		int ret = poll(fd_list, MAX_USERS, POLL_TIMEOUT);
			if (ret == 0)
				continue;
			else if (ret == -1) {
                logger.logMessage("poll returned -1, errno = " + std::string(strerror(errno)), ERROR);
                continue;
            }
			else {
                if (processConnection(listen_sock) < 0)
                    return;
			}
		}
}

Server::~Server() {
}

int Server::getPort() const {
	return this->_port;
}

Server &Server::operator=(const Server &other) {
	this->_port = other._port;
	return(*this);
}