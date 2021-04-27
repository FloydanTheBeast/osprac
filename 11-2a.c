#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int msqid;
	char pathname[] = "11-2a.c";
	key_t key;
	int size;

	const int msgtype1 = 1, msgtype2 = 2; // Типы сообщений
	const int msgnum = 10;
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
		mybuf.mtype = msgtype1;
		strcpy(mybuf.msg, "Message from 11-2a");
		size = sizeof(mybuf.msg);

		// Отправка сообщения
		if (msgsnd(msqid, (struct msgbuf *)&mybuf, size, 0) < 0)
		{
			printf("Can\'t send message to queue\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
			exit(-1);
		}

		mybuf.mtype = msgtype2;

		// Получение сообщения
		if ((size = msgrcv(msqid, (struct msgbuf *)&mybuf, sizeof(mybuf), msgtype2, 0)) < 0)
		{
			printf("Can\'t receive message from queue\n");
			exit(-1);
		}

		printf("11-2a received a message: %s\n", mybuf.msg);
	}

	msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);

	return 0;
}