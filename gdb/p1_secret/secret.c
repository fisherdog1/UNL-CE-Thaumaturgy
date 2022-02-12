#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char** argv)
{
	srand(time(NULL));
	int secret = rand();
		
	while (1)
	{
		printf("Enter Secret Number: ");
		
		int guess;
		scanf("%d15", &guess);

		if (guess == secret)
		{
			printf("That's correct!\n");
			break;
		}
		else
			printf("No!\n");
		
	}
	exit(1);
}