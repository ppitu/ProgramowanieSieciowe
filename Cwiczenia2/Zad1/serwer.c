#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Nie podano numer portu!!!\n");
		exit(EXIT_FAILURE);
	}

	int port;

	struct sockaddr_in serv_addr;
	int addrlen = sizeof(serv_addr);
	char sendBuff[1024];
	int sendBuffSize = sizeof(sendBuff);

	if(1 != sscanf(argv[1], "%d", &port))
	{
		printf("Zly port\n");
		exit(EXIT_FAILURE);
	}

	int socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
	{
		printf("Nie mozna stworzyc gniazda: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', addrlen);
	memset(sendBuff, '0', sendBuffSize);

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

	while(1)
	{
		int accept_desc = accept(socket_desc, (struct sockaddr*)NULL, NULL);

		if(accept_desc == -1)
		{
			printf("Blad accept: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		
		snprintf(sendBuff, sendBuffSize, "%s\r\n", "Hello world");

		if((write(accept_desc, sendBuff, strlen(sendBuff))) == -1)
		{
			printf("Blad write: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		
		if((close(accept_desc)) == -1)
		{
			printf("Blad close: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	if((close(socket_desc)) == -1)
	{
		printf("Blad close: %s\n", strerror(errno));
		exit(EXIT_FAILURE);

	}
}
