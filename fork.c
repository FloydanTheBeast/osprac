#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	pid_t pid = fork();

	if (pid == -1)
	{
		printf("Произошла ошибка");
	}
	else if (pid == 0)
	{
		printf("Дочерний процесс, pid = %d\n", (int)getpid());
	}
	else
	{
		printf("Родительский процесс, pid = %d\n", (int)getpid());
	}

	return 0;
}