#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	printf("UID: %d\n", (int)getuid());
	printf("GID: %d\n", (int)getgid());
	return 0;
}