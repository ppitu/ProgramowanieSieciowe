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


bool drukowalne(const char * buf)
{
	int i = 0;
	int pom = 0;

	while(true)
	{
		//if(*((char*)(buf) + i) >= 32 && *((char *)(buf) + i) <= 126 && *((char*)(buf) + i) != 10)
		if(((char *)buf)[i] >= 32 && ((char *)buf)[i] <= 126 && ((char *)buf)[i] && ((char*)buf)[i] != 10)
		{
			i++;  
		}	
		else
		{
			pom++;
			break;
		}
	}

	if(pom)
		return true;
	else
		return false;
}


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
	char buff[1024];
	int buffSize = sizeof(buff);

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

	while((read_desc = read(socket_desc, buff, buffSize)) > 0)
	{

		/*if(write_desc < 0)
		{
			printf("Blad funkcja write: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}*/

		if(drukowalne(buff))
			printf("%s", buff);
		else
		{
			printf("Nie drukowalne znaki");
			exit(EXIT_FAILURE);
		}
	}

	if(read_desc == -1)
	{
		printf("Read blad: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if(close(socket_desc) == -1)
	{
		printf("Blad close: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
