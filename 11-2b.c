#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
	int msqid;
	char pathname[] = "11-2a.c";
	key_t key;
	int size, maxSize;

	const int msgnum = 10;				  // Количество сообщений
	const int msgtype1 = 1, msgtype2 = 2; // Типы сообщений

	struct mymsgbuf
	{
		long mtype;
		char msg[81];
	} mybuf;

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	for (int i = 0; i < msgnum; i++)
	{
		//
		// In an infinite loop, accept messages of any type in FIFO order
		// with a maximum length of 81 characters
		// until a message of type LAST_MESSAGE is received.
		//

		maxSize = 81;

		if ((size = msgrcv(msqid, (struct msgbuf *)&mybuf, maxSize, msgtype1, 0)) < 0)
		{
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}


		printf("11-2b received a message: %s\n", mybuf.msg);

		// Отправка сообщения
		mybuf.mtype = msgtype2;
		strcpy(mybuf.msg, "Message from 11-2b");
		size = sizeof(mybuf.msg);

		if (msgsnd(msqid, (struct msgbuf *)&mybuf, size, 0) < 0)
		{
			printf("Can\'t send message to queue\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
			exit(-1);
		}
	}

	return 0;
}