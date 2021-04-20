#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/*
	Алгоритм: 
		Родитель:
			for {
				Z(S);
				write
				A(S, 2);
				Z(S);
				read
			}
		Ребенок:
			for {
				D(S, -1);
				read
				D(S, -1);
				write
			}
*/

int main()
{
	int N;
	printf("Enter N: ");
	scanf("%d", &N);

	if (N < 2)
	{
		printf("N must be greater than 1");
		exit(-1);
	}

	int fd[2], fd2[2], result;

	size_t size;
	char resstring1[18], resstring2[17];

	int semid;
	char pathname[] = "09-2.c";

	key_t key;
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_flg = 0;

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can't generate key\n");
		exit(-1);
	}

	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can't create semaphore set\n");
		exit(-1);
	}

	if (pipe(fd) < 0)
	{
		printf("Can\'t open pipe\n");
		exit(-1);
	}

	if (pipe(fd2) < 0)
	{
		printf("Can\'t open pipe\n");
		exit(-1);
	}

	result = fork();

	if (result < 0)
	{
		printf("Can\'t fork child\n");
		exit(-1);
	}
	else if (result > 0)
	{
		/* Parent process */
		for (int i = 0; i < N; i++)
		{
			buf.sem_op = 0;

			if (semop(semid, &buf, 1) < 0)
			{
				printf("parent: can't wait for condition\n");
				exit(-1);
			}

			size = write(fd[1], "Hello from parent!", 18);

			if (size != 18)
			{
				printf("parent: Can\'t write all string to pipe\n");
				exit(-1);
			}

			buf.sem_op = 2;
			if (semop(semid, &buf, 1) < 0)
			{
				printf("parent: can't wait for condition\n");
				exit(-1);
			}

			buf.sem_op = 0;
			if (semop(semid, &buf, 1) < 0)
			{
				printf("Can't wait for condition\n");
				exit(-1);
			}

			size = read(fd2[0], resstring2, 17);

			if (size < 0)
			{
				printf("parent: Can\'t read string from pipe\n");
				exit(-1);
			}

			printf("Parent received a message: %s\n", resstring2);
		}

		if (close(fd[0]) < 0)
		{
			printf("parent: Can\'t close reading side of pipe\n");
			exit(-1);
		}

		if (close(fd2[1]) < 0)
		{
			printf("parent: Can\'t close writing side of pipe");
			exit(-1);
		}

		if (close(fd[1]) < 0)
		{
			printf("parent: Can\'t close writing side of pipe\n");
			exit(-1);
		}

		if (close(fd2[0]) < 0)
		{
			printf("parent: Can\'t close reading side of pipe\n");
			exit(-1);
		}
	}
	else
	{
		/* Child process */

		for (int i = 0; i < N; i++)
		{
			buf.sem_op = -1;

			if (semop(semid, &buf, 1) < 0)
			{
				printf("child: can't wait for condition");
			}

			size = read(fd[0], resstring1, 18);

			if (size < 0)
			{
				printf("child: Can\'t read string from pipe\n");
				exit(-1);
			}

			printf("Child received a message: %s\n", resstring1);

			buf.sem_op = -1;

			if (semop(semid, &buf, 1) < 0)
			{
				printf("child: can't wait for condition");
			}

			size = write(fd2[1], "Hello from child!", 17);

			if (size != 17)
			{
				printf("child: Can't write all string to pipe\n");
			}
		}

		if (close(fd[1]) < 0)
		{
			printf("child: Can\'t close writing side of pipe\n");
			exit(-1);
		}

		if (close(fd2[0]) < 0)
		{
			printf("child: Can\'t close reading side of pipe\n");
			exit(-1);
		}

		if (close(fd[0]) < 0)
		{
			printf("child: Can\'t close reading side of pipe\n");
			exit(-1);
		}

		if (close(fd2[1]) < 0)
		{
			printf("child: Can\'t close writing side of the pipe\n");
			exit(-1);
		}
	}

	return 0;
}