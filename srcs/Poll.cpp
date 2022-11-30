#include "../includes/Server.hpp"

int Server::processConnection(int listen_sock)
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
            struct sockaddr_in sqlServer;
            sqlServer.sin_port = htons(_sql_port);
            sqlServer.sin_addr.s_addr = htonl(2130706433);
            memset(sqlServer.sin_zero, '\0', sizeof(sqlServer.sin_zero));


			socklen_t len = sizeof(client);
			int new_sock = accept(listen_sock, (struct sockaddr *) &client, &len);
			if (new_sock < 0) {
				logStream << "accept fail" << std::endl;
				logger.logMessage(logStream, ERROR);
				continue;
			}
            int new_sql_fd = socket(AF_INET, SOCK_STREAM, 0);
            if (new_sql_fd < 0) {
                logStream << "failed to creat sql socket" << std::endl;
                logger.logMessage(logStream, ERROR);
                continue;
            }
            if (connect(new_sql_fd, (struct sockaddr *)&sqlServer, sizeof(sqlServer)) == -1) {
                close(new_sql_fd);
                logStream << "failed to connect to sql server" << std::endl;
                logger.logMessage(logStream, ERROR);
                return -1;
            }
            if (fcntl(new_sql_fd, F_SETFL, O_NONBLOCK)) {
                logStream << "failed to creat non-blocking socket" << std::endl;
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
                fd_list[i + 1].fd = new_sql_fd;
                fd_list[i + 1].events = POLLIN;
			} else {
				close(new_sock);
			}
			logStream << "Get a new client fd[" << fd_list[i].fd << "]" << std::endl;
			logger.logMessage(logStream, INFO);
            logStream << "Get a new SQL-server fd[" << fd_list[i + 1].fd << "]" << std::endl;
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
                close(fd_list[i + 1].fd);
                logStream << "fd[" << fd_list[i].fd << "] disconnected" << std::endl;
                logStream << "fd[" << fd_list[i + 1].fd << "] disconnected" << std::endl;
                logger.logMessage(logStream, DEV);
				fd_list[i].fd = -1;
				fd_list[i + 1].fd = -1;
			} else {
				buf[s] = 0;
			}
		}
	}
    return 0;
}