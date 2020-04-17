#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 2021

int main(int argc, char **argv)
{

	struct sockaddr_in serv_addr, cli_addr;
	int addrlen = sizeof(serv_addr);
	socklen_t clilen = sizeof(cli_addr);
	char sendBuff[1024];

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
	serv_addr.sin_port = htons(PORT);

	if((bind(socket_desc, (struct sockaddr*)&serv_addr, addrlen)) == -1)
	{
		printf("Blad bind: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int recv_len = 0;

	char pomtab[500000] = "";		
	char send[1024] = "";
	while((recv_len = recvfrom(socket_desc, (char *)sendBuff, 1024, MSG_WAITALL, (struct sockaddr *)&cli_addr, &clilen)) > 0)
	{

		if(recv_len == -1)
		{
			printf("recvfrom blad: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		printf("%s", sendBuff);

		sendBuff[recv_len] = '\0';

		int i = 0;
		int wynik = 0;
		int liczba;
		int z = 0;

		while(sendBuff[i] != '\0')
		{
			if(sendBuff[i] == ' ')
			{
				sscanf(pomtab, "%d", &liczba);
				wynik += liczba;
				liczba = 0;
				z = 0;
				continue;
			}

			pomtab[z] = sendBuff[i];
			z++;
			i++;
		}

		
		snprintf(send, sizeof(send), "%d", wynik);
		
		int send_len = sendto(socket_desc, (const char *)send, strlen(send), MSG_CONFIRM, (struct sockaddr *)&cli_addr, clilen);

		if(send_len == -1)
		{
			printf("sendto blad: %s\n", strerror(errno));
			exit(EXIT_FAILURE);

		}

		
	}
	if(close(socket_desc) == -1)
	{
		printf("close blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

}
