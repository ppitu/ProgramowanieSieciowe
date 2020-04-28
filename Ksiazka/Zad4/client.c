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
#include <pthread.h>

#define MAXLINE 1024

static int socket_desc;

void *copyto(void *arg);

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Nie podano numeru portu lub\\i adresu IP\n");
		exit(EXIT_FAILURE);
	}

	int port;

	struct sockaddr_in 	serv_addr;
	char			recvline[MAXLINE];
	int 			read_desc;
	pthread_t		tid;

	if(1 != sscanf(argv[2], "%d", &port))
	{
		printf("sscanf error!!!");
		exit(EXIT_FAILURE);
	}

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
	{
		printf("socket error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(port);

	if(connect(socket_desc, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		printf("connect error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	/******************************************************************/
	
	if(pthread_create(&tid, NULL, copyto, NULL) > 0)
	{
		printf("pthread_create error: %s\n");
		exit(EXIT_FAILURE);
	}

	while(read(socket_desc, recvline, MAXLINE) > 0)
	{

		fputs(recvline, stdout);

	}

}

void *copyto(void *arg)
{
	char 	sendline[MAXLINE];	

	while(fgets(sendline, MAXLINE, stdin) != NULL)
		write(socket_desc, sendline, strlen(sendline));

	shutdown(socket_desc, SHUT_WR);

	return(NULL);
	
}
