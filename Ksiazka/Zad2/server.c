#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/wait.h>


int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Nie podano numeru portu!!!\n");
		exit(EXIT_FAILURE);
	}

	int port;

	if(1 != sscanf(argv[1], "%d", &port))
	{
		printf("Zly port\n");
		exit(EXIT_FAILURE);
	}	

	struct sockaddr_in serv_addr, chil_addr;
	int addrlen = sizeof(serv_addr);
	int i, maxi, maxfd, sockfd;
	int socket_desc, accept_desc;
	int nready, client[FD_SETSIZE];
	fd_set rset, allset;
	socklen_t chilen;
	char buff[1024];
	int read_desc = 0;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc == -1)
	{
		printf("nie mozna stworzyc gniazda: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	memset(&serv_addr, '0', addrlen);
	memset(buff, '0', sizeof(buff));

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

	maxfd = socket_desc;
	maxi = -1;
	for(i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;

	FD_ZERO(&allset);
	FD_SET(socket_desc, &allset);

	while(1) 
	{
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

		if(FD_ISSET(socket_desc, &rset)) //nowe polaczenie z klientem
		{

			chilen = sizeof(addrlen);

			accept_desc = accept(socket_desc, (struct sockaddr*)&chil_addr, &chilen);

			if(accept_desc == -1)
			{
				printf("Blad accept: %s\n", strerror(errno));
				exit(EXIT_FAILURE);
			}

			for(i = 0; i < FD_SETSIZE; i++)
			{
				if(client[i] < 0)
				{
					client[i] = accept_desc;
					break;
				}
			}

			if(i == FD_SETSIZE)
				printf("too many clients");
			
			FD_SET(accept_desc, &allset); //dodajemy nowy deskryptor

			if(accept_desc > maxfd)
				maxfd = accept_desc;

			if(i > maxi)
				maxi = i;

			if(--nready <= 0) //nie ma deskryptoru do czytanie
				continue;

		}

		for(i = 0; i <= maxi; i++)
		{
			if((sockfd = client[i]) < 0)
				continue;

			if(FD_ISSET(sockfd, &rset))
			{
				read_desc = read(accept_desc, buff, sizeof(buff));
				if(read_desc < 0)
				{
					printf("Blad read: %s\n", strerror(errno));
					exit(EXIT_FAILURE);
				}

				if(read_desc == 0) //zamkniecie polaczenia
				{
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} else {
					buff[read_desc] = '\0';
					printf("%s", buff);
					if((write(accept_desc, buff, strlen(buff))) == -1)
					{
						printf("Blad write: %s\n", strerror(errno));
						exit(EXIT_FAILURE);
					}

				}

				if(--nready <= 0)
					break; //nie ma dostepnych deskryptorow
	
			}
		}


	}

}
