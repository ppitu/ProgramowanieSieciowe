#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

bool drukowalne(const char * buf)
{
	int i = 0;
	int pom = 0;

	while(true)
	{
		//if(*((char*)(buf) + i) >= 32 && *((char *)(buf) + i) <= 126 && *((char*)(buf) + i) != 10)
		if(((char *)buf)[i] >= 32 && ((char *)buf)[i] <= 126 && ((char *)buf)[i] && ((char*)buf)[i] != 10)
			i++;  	
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

int main()
{
	char tab[] = "Alama";

	bool x = drukowalne(tab);

	if(x)
		printf("Jest drukowalne\n");
	else
		printf("Nie jest\n");
}
