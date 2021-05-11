#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main() {
	int msqid;
	char pathname[] = "12-server.c";
	key_t key;
	int len;

	struct buff
	{
		long mtype;
		struct {
			int pid;
			double number;
		} payload;
	} msgbuff;

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Server: Couldn't generate key\n");
		exit(-1);
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if (errno == EEXIST) {
			printf("Error: Server instance is already running\n");
			exit(-1);
		} else {
			printf("Error: Couldn't get msqid\n");
			exit(-1);
		}
	}

	while (1) {
		if ((len = msgrcv(msqid, (struct buff *)&msgbuff, sizeof(msgbuff.payload), 1, 0)) < 0)
		{
			printf("Server: Couldn't receive a message from queue\n");
			exit(-1);
		}

		// Послано сообщение от KILLER
		if (msgbuff.payload.pid == 2) {
			printf("Server: received a message from killer, shutting down...\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
			exit(0);
		}

		printf("Server: Received a message from client with PID %d", msgbuff.payload.pid);

		// Отправляем сообщение типом, равным полученному
		msgbuff.mtype = msgbuff.payload.pid;
		// Возводим полученное число в квадрат
		msgbuff.payload.number *= msgbuff.payload.number;

		if (msgsnd(msqid, (struct buff *)&msgbuff, sizeof(msgbuff.payload), 0) < 0) {
			printf("Server: Couldn't send message to the queue");
			msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
			exit(-1);
		}

		printf("Server: Responded to client %d", msgbuff.payload.pid);
	}

	return 0;
}