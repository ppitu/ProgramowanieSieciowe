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
		printf("Nie podano numer portu!!!\n");
		exit(EXIT_FAILURE);
	}	

	char liczba[20];
	char liczba1[20];
	int port;
	struct sockaddr_in serv_addr;
	char buffer[1024];
	
	if(1 != sscanf(argv[1], "%d", &port))
	{
		printf("Zly port\n");
		exit(EXIT_FAILURE);
	}

	int socket_desc = socket(AF_INET, SOCK_DGRAM, 0);

	if(socket_desc == -1)
	{
		printf("Blad socket: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	socklen_t len = sizeof(serv_addr);
	

	printf("Podaj pierwsza liczbe: ");
	scanf("%s", liczba);
	printf("Podaj druga liczbe: ");
	scanf("%s", liczba1);

	int sendto_len = sendto(socket_desc, (const char *)liczba, strlen(liczba), MSG_CONFIRM, (struct sockaddr *)&serv_addr, len);

	if(sendto_len == -1)
	{
		printf("Blad sendto: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	int sendto_len1 = sendto(socket_desc, (const char *)liczba1, strlen(liczba1), MSG_CONFIRM, (struct sockaddr *)&serv_addr, len);

	if(sendto_len1 == -1)
	{
		printf("Blad sendto: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}


	int recvfrom_len = 0;

	recvfrom_len = recvfrom(socket_desc, (char *)buffer, 1024, MSG_WAITALL, (struct sockaddr*)&serv_addr, &len);
	

	if(recvfrom_len == -1)
	{
		printf("recvfrom blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("%s", buffer);
	
	if(close(socket_desc) == -1)
	{
		printf("Blad close: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
