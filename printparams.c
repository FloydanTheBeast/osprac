#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
	for (int i = 0; i < argc; i++)
		printf("[arg#%d] %s\n", i + 1, argv[i]);

	printf("\n");
	int i = 0;

	for (char **ptr = envp; *ptr; ptr++)
		printf("[arg#%d] %s\n", ++i, *ptr);

	return 0;
}