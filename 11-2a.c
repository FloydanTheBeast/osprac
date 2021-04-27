#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
	int msqid;					 // IPC descriptor for the message queue
	char pathname[] = "11-2a.c"; // The file name used to generate the key.
								 // A file with this name must exist in the current directory.
	key_t key;					 // IPC key
	int i, size;				 // Cycle counter and the length of the informative part of the message

	const int msgtype1 = 1, msgtype2 = 2; // Типы сообщений
	const int msgnum = 10;
	struct mymsgbuf // Custom structure for the message
	{
		long mtype;
		char msg[81];
	} mybuf;

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}
	//
	// Trying to get access by key to the message queue, if it exists,
	// or create it, with read & write access for all users.
	//
	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	/* Send information */

	for (i = 0; i < msgnum; i++)
	{
		//
		// Fill in the structure for the message and
		// determine the length of the informative part.
		//
		mybuf.mtype = msgtype1;
		strcpy(mybuf.msg, "Message from 11-2a");
		size = sizeof(mybuf.msg);
		//
		// Send the message. If there is an error,
		// report it and delete the message queue from the system.
		//
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