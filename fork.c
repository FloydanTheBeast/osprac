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
		printf("Дочерний процесс, pid = %d, ppid = %d\n", (int)getpid(), (int)getppid());
	}
	else
	{
		printf("Родительский процесс, pid = %d, ppid = %d\n", (int)getpid(), (int)getppid());
	}

	return 0;
}