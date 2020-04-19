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
	char sendLine[1024];
	char recvLine[1024];

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
	while(fgets(sendLine, 1024, stdin) != NULL)
	{

		if((write(socket_desc, sendLine, strlen(sendLine))) == 1)
		{
			printf("Blad write: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		read_desc = read(socket_desc, recvLine, sizeof(recvLine));

		recvLine[read_desc] = '\0';

		if(read_desc == -1)
		{
			printf("Read blad: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		fputs(recvLine, stdout);
	}
	if(close(socket_desc) == -1)
	{
		printf("Blad close: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


}
