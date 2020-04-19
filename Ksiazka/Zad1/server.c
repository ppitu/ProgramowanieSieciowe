#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>

#include "signal.h"

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Nie podano numeru portu!!!\n");
		exit(EXIT_FAILURE);
	}

	int port;

	if(1 != sscanf(argv[1], "%d", &port))
	{
		printf("Zly port\n");
		exit(EXIT_FAILURE);
	}	

	struct sockaddr_in serv_addr, chil_addr;
	int addrlen = sizeof(serv_addr);

	pid_t childpid;
	int socket_desc, accept_desc;
	socklen_t chilen;
	char buff[1024];
	int read_desc = 0;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
	{
		printf("nie mozna stworzyc gniazda: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', addrlen);
	memset(buff, '0', sizeof(buff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	if((bind(socket_desc, (struct sockaddr*)&serv_addr, addrlen)) == -1)
	{
		printf("Blad bind: %s!!!\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if((listen(socket_desc, 10)) == -1)
	{
		printf("Blad listen: %s!!!\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	signal(SIGCHLD, sig_chld);

	while(1) 
	{
		chilen = sizeof(addrlen);

		accept_desc = accept(socket_desc, (struct sockaddr*)&chil_addr, &chilen);

		if(accept_desc == -1)
		{
			if(errno == EINTR)
			{
				continue;
			}

			printf("Blad accept: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}


		childpid = fork();

		if(childpid == -1)
		{
			printf("Blad fork: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if(childpid == 0)
		{
			close(socket_desc);
			while(1){
				read_desc = read(accept_desc, buff, sizeof(buff));
				if(read_desc < 0)
				{
					printf("Blad read: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				if(read_desc == 0)
					break;

				buff[read_desc] = '\0';
				printf("%s", buff);
				if((write(accept_desc, buff, strlen(buff))) == -1)
				{
					printf("Blad write: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
			}
			exit(0);

		}

		close(accept_desc);

	}

}
