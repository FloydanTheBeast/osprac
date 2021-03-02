#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>

int main()
{
    int bufferSize = 0;
    int fd[2];
    size_t size = 1;
    char *str = malloc(sizeof(char));

    if (pipe(fd) < 0)
    {
        printf("Can't open pipe\n");
        exit(-1);
    }

    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

    // Пытаемся записать в буффер, пока это возможно
    while (size == 1)
    {
        size = write(fd[1], str, 1);
        bufferSize++;
    }

    printf("Size of pipe's buffer is %d bytes\n", bufferSize);
    return 0;
}
