#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int *array;
    int shmid;
    char path[] = "7-01.c";
    key_t key;
    char *sourceCode;

    int fileSize = 3000;

    if ((key = ftok(path, 0)) < 0)
    {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, fileSize * sizeof(char), 0)) < 0)
    {
        printf("Can't create shared memory\n");
        exit(-1);
    }

    if ((sourceCode = (char *)shmat(shmid, NULL, 0)) == (char *)(-1))
    {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    for (int i = 0; i < fileSize; i++)
        printf("%c", sourceCode[i]);

    if (shmdt(sourceCode) < 0)
    {
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) < 0)
    {
        printf("Can't delete shared memory\n");
        exit(-1);
    }
    return 0;
}