#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd, result;
    size_t size;
    char str[14];
    char name[] = "data.fifo";
    (void)umask(0);

    if (mknod(name, S_IFIFO | 0666, 0) < 0)
    {
        printf("Can't create FIFO file\n");
        exit(-1);
    }

    printf("data.fifo has been created created\nReader starts working\n");

    if ((fd = open(name, O_RDONLY)) < 0)
    {
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }

    size = read(fd, str, 14);

    if (size < 0)
    {
        printf("Can\'t read string\n");
        exit(-1);
    }

    printf("Data received from FIFO: %s\n", str);

    if (close(fd) < 0)
    {
        printf("Can't close reader\n");
        exit(-1);
    }

    printf("Reader closed\n");

    return 0;
}
