#include "lib.h"

#define MAXLINE 1024

static void *doit(void *arg)
{
	ssize_t		n;
	char 		line[MAXLINE];
	int 		connfd;


	connfd = *((int *) arg);
	free(arg);

	pthread_detach(pthread_self());

	while(1)
	{
		if((n = read(connfd, line, MAXLINE)) == 0)
			return;

		write(connfd, line, n);
	}

	close(connfd);
	return(NULL);
}


int main(int argc, char**argv)
{
	int			socket_desc, *connect_desc, err;
	socklen_t		addrlen, len;
	struct sockaddr 	*cliaddr;
	pthread_t		thid;

	if(argc == 2)
		socket_desc = tcp_listen(NULL, argv[1], &addrlen);
	else if(argc == 3)
		socket_desc = tcp_listen(argv[1], argv[2], &addrlen);
	else
	{
		printf("error <service or port>\n");
		exit(EXIT_FAILURE);
	}

	cliaddr = malloc(addrlen);

	for( ; ; )
	{
		len = addrlen;
		connect_desc = malloc(sizeof(int));
		*connect_desc = accept(socket_desc ,cliaddr, &len);
		if((err = pthread_create(&thid, NULL, &doit, connect_desc)) != 0)
		{
			printf("pthread_create error: %s\n", gai_strerror(err));
			exit(EXIT_FAILURE);
		}
	}
}


