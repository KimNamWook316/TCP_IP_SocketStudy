#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void main(int argc, char* argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d] : %s\n", i, argv[i]);
	}
	if (argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	WSADATA wsaData = {};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartUp() Error");
		exit(1);
	}

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
	{
		printf("socket() Error");
		exit(1);
	}

	SOCKADDR_IN	servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("connect() Error");
		exit(1);
	}
	else
	{
		printf("Connected.........\n");
	}

	int msgLen = 0;
	char buf[BUF_SIZE - 4] = {};
	char msg[BUF_SIZE] = {};
	char servMsg[BUF_SIZE] = {};

	for (int i = 0; i < 3; ++i)
	{
		int recvCnt = 0;
		int recvLen = 0;

		while (recvLen < sizeof(int))
		{
			recvCnt = recv(hSocket, &servMsg[recvLen], sizeof(int), 0);

			if (recvCnt == -1)
			{
				printf("recv() Error.");
				break;
			}

			recvLen += recvCnt;
		}

		int clntMsgLen = servMsg[0];

		recvCnt = 0;
		recvLen = 0;

		while (recvLen < clntMsgLen)
		{
			recvCnt = recv(hSocket, &servMsg[recvLen], BUF_SIZE - 4, 0);

			if (recvCnt == -1)
			{
				printf("recv() Error.");
				break;
			}

			recvLen += recvCnt;
		}

		servMsg[recvLen] = '\0';
		printf("Message From Server : %s\n", servMsg);

		sprintf_s(buf, "This is Message From Client %d.", i);

		msgLen = strlen(buf);

		memcpy(msg, &msgLen, sizeof(int));
		memcpy(&msg[4], buf, msgLen);

		if (send(hSocket, msg, msgLen + sizeof(int), 0) == -1)
		{
			printf("send() Error.");
			break;
		}

	}

	closesocket(hSocket);

	printf("Quit Client.\n");
	WSACleanup();
}
