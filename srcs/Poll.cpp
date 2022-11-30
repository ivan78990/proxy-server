#include "../includes/Server.hpp"

void Server::processConnection(int listen_sock)
{
	std::stringstream logStream;

	int i = 0;
	for (; i < MAX_USERS; i++) {
		if (fd_list[i].fd == -1) {
			continue;
		}
		if (fd_list[i].fd == listen_sock &&
			(fd_list[i].revents & POLLIN)) {
			struct sockaddr_in client;
			socklen_t len = sizeof(client);
			int new_sock = accept(listen_sock, (struct sockaddr *) &client, &len);
			if (new_sock < 0) {
				logStream << "accept fail" << std::endl;
				logger.logMessage(logStream, ERROR);
				continue;
			}

			int i = 0;
			for (; i < MAX_USERS; i++) {
				if (fd_list[i].fd == -1)
					break;
			}
			if (i < MAX_USERS) {
				fd_list[i].fd = new_sock;
				fd_list[i].events = POLLIN;
			} else {
				close(new_sock);
			}
			logStream << "Get a new link fd[" << fd_list[i].fd << "] IP " <<
					  inet_ntoa(client.sin_addr) << ":" <<
					  ntohs(client.sin_port) << std::endl;
			logger.logMessage(logStream, INFO);
			continue;
		}
		if (fd_list[i].revents & POLLIN) {
			char buf[1024];
			ssize_t s = read(fd_list[i].fd, buf, sizeof(buf) - 1);
			if (s < 0) {
				logStream << "read fail" << std::endl;
				logger.logMessage(logStream, ERROR);
			} else if (s == 0) {
				close(fd_list[i].fd);
                logStream << "Client fd[" << fd_list[i].fd << "] disconnected" << std::endl;
                logger.logMessage(logStream, DEV);
				fd_list[i].fd = -1;
			} else {
				buf[s] = 0;
			}
		}
	}
}