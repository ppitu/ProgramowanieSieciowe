#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUF_SIZE 1

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Nie podano odpowidniej liczby plikow!!!");
		exit(EXIT_FAILURE);
	}

	char *zrodlowy = argv[1];
	char *docelowy = argv[2];

	int fd1 = open(zrodlowy, O_RDONLY);

	if(fd1 < 0) //Blad otwarcia pliku
	{
		printf("Nie mozna otworzyc pliku zrodlowego %s: %s\n", zrodlowy, strerror(errno));
		exit(EXIT_FAILURE);
	}

	int fd2 = open(docelowy, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	
	if(fd2 < 0) //Blad otwacia pilku
	{
		printf("nie mozna otworzyc pliku docelowego %s: %s\n", docelowy, strerror(errno));
	}

	printf("Kopiuje z %s do %s.\n", zrodlowy, docelowy);

	char buf[BUF_SIZE] = {0}; //Zmienna na przechowywane dane

	int readd = 0;
	int wwrite = 0;
	int sem = 0;

	while((readd = read(fd1, buf, BUF_SIZE)) != 0)
	{
		//Modyfikacja do kolejnego zadania
		if((*(char *)buf) == 10 && sem == 0)
			sem++;

		if((*(char *)buf) == 10 && sem != 0)
			sem--;

		
		if(sem == 0)
		{
		wwrite = write(fd2, buf, BUF_SIZE);

		if(wwrite < 0)
		{
			printf("write() blad: %s\n", strerror(errno));
			close(fd1);
			close(fd2);
			exit(EXIT_FAILURE);
		}
		}
	}


	if(readd < 0)
	{
		printf("read() blad: %s\n", strerror(errno));
		close(fd1);
		close(fd2);
		exit(EXIT_FAILURE);
	}

	close(fd1);
	close(fd2);
	
}
