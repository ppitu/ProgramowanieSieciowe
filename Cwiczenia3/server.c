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
#include <limits.h>

#define PORT 2062

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
	while(1)
	{

		recv_len = recvfrom(socket_desc, (char *)sendBuff, 1024, MSG_WAITALL, (struct sockaddr *)&cli_addr, &clilen);
		{

			printf("Receive\n");

			if(recv_len == -1)
			{
				printf("recvfrom blad: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}


			sendBuff[recv_len] = '\0';

			int i = 0;
			long wynik = 0;
			long liczba = 0;
			int z = 0;
			int blad = 0;

			while(sendBuff[i] != '\0')
			{
				if(sendBuff[i] == ' ')
				{
					sscanf(pomtab, "%ld", &liczba);
					if(liczba >= LONG_MAX || liczba <= LONG_MIN)
					{
						blad++;
					}
					wynik += liczba;

					if(wynik >= LONG_MAX || wynik <= LONG_MIN)
					{
						blad++;
					}

					liczba = 0;
					z = 0;
					i++;
					memset(pomtab, 0, sizeof(pomtab));
					continue;
				}
				if(blad != 0)
				{
					break;
				}

				pomtab[z] = sendBuff[i];
				z++;
				i++;
			}


		
			sscanf(pomtab, "%ld", &liczba);
			wynik += liczba;

			if(wynik >= LONG_MAX || wynik <= LONG_MAX)
			{
				blad++;
			}

			if(blad != 0)
			{

			snprintf(send, sizeof(send), "%ld", wynik);
			}
			else {
				strcpy(send, "Overflow\n");	
			}

			printf("Send\n");

			int send_len = sendto(socket_desc, (const char *)send, strlen(send), MSG_CONFIRM, (struct sockaddr *)&cli_addr, clilen);

			if(send_len == -1)
			{
				printf("sendto blad: %s\n", strerror(errno));
				exit(EXIT_FAILURE);

			}


		}
	}
	if(close(socket_desc) == -1)
	{
		printf("close blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

}
