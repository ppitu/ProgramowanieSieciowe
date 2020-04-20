#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/select.h>
#include <stdlib.h>

#define MAXLINE 1024



#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */




int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Nie podano portu lub\\i adresu IP\n");
		exit(EXIT_FAILURE);
	}

	int port;

	struct sockaddr_in serv_addr;
	int addrlen = sizeof(serv_addr);
	char sendLine[MAXLINE];
	char recvLine[MAXLINE];


	int maxfdp1;
	fd_set	reset;
	int stdineof = 0;


	if(1 != sscanf(argv[2], "%d", &port))
	{
		printf("W porcie podano inne znaki niz cyfry!!!\n");
		exit(EXIT_FAILURE);
	}

	int socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
	{
		printf("Nie mozna stworzyc gniazda: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', addrlen);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(port);

	if(connect(socket_desc, (struct sockaddr*)&serv_addr, addrlen) == -1)
	{
		printf("Nie mozna nawiazac polaczenia z serwerem: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int read_desc = 0;

	/*********************************/
	FD_ZERO(&reset);
	while(1)
	{
		if(stdineof == 0)
		{
			FD_SET(fileno(stdin), &reset);
		}

		FD_SET(socket_desc, &reset);

		maxfdp1 = max(fileno(stdin), socket_desc) + 1;
		select(maxfdp1, &reset, NULL, NULL, NULL);

		if(FD_ISSET(socket_desc, &reset)) //gniazdo gotowe do czytania
		{
			read_desc = read(socket_desc, recvLine, sizeof(recvLine));

			if(stdineof == 1)
			{
				break;
			}

			if(read_desc == -1)
			{
				printf("Read blad: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}


			recvLine[read_desc] = '\0';
			fputs(recvLine, stdout);
		}

		if(FD_ISSET(fileno(stdin), &reset)) //wejscie gotowe do czytania
		{
			if(fgets(sendLine, MAXLINE, stdin) == NULL)
			{
				stdineof = 1;
				if((shutdown(socket_desc, SHUT_WR) == -1)) //wyslanie FIN
				{
					printf("shutdown error: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}
				FD_CLR(fileno(stdin), &reset);
				continue;

			}
			
			if((write(socket_desc, sendLine, strlen(sendLine))) == 1)
			{
				printf("Blad write: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}
	
		}
	}

	if(close(socket_desc) == -1)
	{
		printf("Blad close: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


}
