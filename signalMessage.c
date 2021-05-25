#include <signal.h>
#include <stdio.h>

void my_handler(int nsig)
{
	switch (nsig) {
		case SIGINT:
			printf("Received a signal %d, CTRL-C was pressed\n", nsig);
			break;
		case SIGQUIT:
			printf("Received a signal %d, CTRL-4 was pressed\n", nsig);
			break;
	}
}

int main(void)
{
	(void)signal(SIGINT, my_handler);
	(void)signal(SIGQUIT, my_handler);
	
	while (1);
	return 0;
}
