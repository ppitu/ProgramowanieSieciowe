#include <stdio.h>

int main()
{
	int liczby[50];
	int i, liczba;

	for(i = 0; i < 50; i++)
	{
		scanf("%d", &liczba);

		if(liczba == 0)
			break;

		*(liczby + i) = liczba;
	}
	
	
	for(i = 0; i < 50; i++)
	{
		if(*(liczby + i) < 100 && *(liczby + i) > 10)
			printf("%d ", *(liczby+i));
	}

	printf("\n");

	return 0;
}
