#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int fd, result;
    size_t size;
    char str[14];
    char name[] = "data.fifo";
    (void)umask(0);

    printf("Writer starts working\n");

    if ((fd = open(name, O_WRONLY)) < 0)
    {
        printf("Can't open FIFO for writing\n");
        exit(-1);
    }

    size = write(fd, "Hello, world!", 14);
    printf("Writer finished working\n");

    if (size != 14)
    {
        printf("Can\'t write all string to FIFO\n");
        exit(-1);
    }

    if (close(fd) < 0)
    {
        printf("Can't close writer\n");
        exit(-1);
    }

    printf("Writer closed\n");
    return 0;
}
