#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool drukowalne(const void * buf, int len)
{
	int i;
	int pom = 0;

	for(i = 0; i < len; i++)
	{
		//if(*((unsigned char*)(buf) + i) >= 32 && *((unsigned char*)(buf) + i) <= 126)
		if(((unsigned char*)buf)[i] >= 32 && ((unsigned char*)buf)[i] <= 126 )
			continue;
		else
			pom++;
	}	

	if(pom != 0)
		return false;
	return true;
}

int main()
{
	char tab[] = "Alams";

	bool x = drukowalne(tab, 6);

	if(x)
		printf("Jest drukowalna\n");
	else
		printf("Nie jest\n");
	

	return 0;
}
