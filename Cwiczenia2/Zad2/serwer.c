#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Nie podano numeru portu!!!\n");
		exit(EXIT_FAILURE);
	}

	int port;

	struct sockaddr_in serv_addr, cli_addr;
	int addrlen = sizeof(serv_addr);
	socklen_t clilen = sizeof(cli_addr);
	char sendBuff[1024];
	//int sendBuffSize = sizeof(sendBuff);

	if(1 != sscanf(argv[1], "%d", &port))
	{
		printf("Zlu port!!!\n");
		exit(EXIT_FAILURE);
	}

	int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);

	if(socket_desc == -1)
	{
		printf("Nie mozna stworzyc gniazda: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', addrlen);
	memset(&cli_addr, '0', clilen);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	if((bind(socket_desc, (struct sockaddr*)&serv_addr, addrlen)) == -1)
	{
		printf("Blad bind: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int recv_len = recvfrom(socket_desc, (char *)sendBuff, 1024, MSG_WAITALL, (struct sockaddr *)&cli_addr, &clilen);

	if(recv_len == -1)
	{
		printf("recvfrom blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int send_len = sendto(socket_desc, "Hello server\n", strlen("Hello server\n"), MSG_CONFIRM, (struct sockaddr *)&cli_addr, clilen);
	
	if(send_len == -1)
	{
		printf("sendto blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(close(socket_desc) == -1)
	{
		printf("close blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

}
