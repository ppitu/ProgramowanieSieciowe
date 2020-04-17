#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{

	char tab[20] = "123 453 1276 123 34";

	char pomtab[5000000] = "";

	int i = 0;
	int wynik = 0;
	int liczba;
	int z = 0;
	
	while(tab[i] != '\0')
	{
		if(tab[i] == ' ')
		{
			sscanf(pomtab, "%d", &liczba);
			wynik += liczba;
			liczba = 0;
			i++;
			z = 0;
			continue;
		}

		pomtab[z] = tab[i];
		z++;
		i++;
	}

	printf("%d", wynik);

	return 0;
}
