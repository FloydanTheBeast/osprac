#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
    int shmid;
    char path[] = "7-01.c";
    const int fileSize = 3000;
    int file = open(path, O_RDONLY);

    if (file < 0)
    {
        printf("Can't open source file");
        exit(-1);
    }

    key_t key;
    char *sourceCode;

    if ((key = ftok(path, 0)) < 0)
    {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((shmid = shmget(key, fileSize * sizeof(char), 0666 | IPC_CREAT)) < 0)
    {
        printf("Can't create shared memory\n");
        exit(-1);
    }
    if ((sourceCode = (char *)shmat(shmid, NULL, 0)) == (char *)(-1))
    {
        printf("Can't attach shared memory\n");
        exit(-1);
    }

    // Читаем файл в буффер
    read(file, sourceCode, fileSize);

    if (shmdt(sourceCode) < 0)
    {
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    return 0;
}