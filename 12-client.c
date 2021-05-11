#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	int msqid;
	char pathname[] = "12-server.c";
	key_t key;
	int len;
	int PID = (int)getpid();

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
		printf("Client %d: Couldn't generate key\n", PID);
		exit(-1);
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Client %d: Couldn't get msqid\n", PID);
		exit(-1);
	}

	while (1) {
		msgbuff.mtype = 1;
		// Записываем PID процесса для проверки на сервере
		msgbuff.payload.pid = PID;
		printf("Client %d: Enter double number: ", PID);
		scanf("%lf", &msgbuff.payload.number);

		if (msgsnd(msqid, (struct buff *)&msgbuff, sizeof(msgbuff.payload), 0) < 0) {
			printf("Client %d: Couldn't send message to the queue\n", PID);
			msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
			exit(-1);
		}

		printf("Client %d: Sent message to the server\n", PID);

		
		if ((len = msgrcv(msqid, (struct buff *)&msgbuff, sizeof(msgbuff.payload), PID, 0)) < 0)
		{
			printf("Client: Couldn't receive a message from queue\n");
			exit(-1);
		}

		printf("Client %d: Server responded with a square of an entered number: %0.3lf\n", PID, msgbuff.payload.number);
	}

	return 0;
}