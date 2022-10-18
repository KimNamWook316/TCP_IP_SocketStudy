#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void main(int argc, char* argv[])
{
	WSADATA wsaData = {};

	SOCKET hServSock, hClntSock;

	SOCKADDR_IN	servAddr, clntAddr;
	int clntAddrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartUp() Error");
		exit(1);
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
	{
		printf("socket() Error");
		exit(1);
	}

	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind() Error");
		exit(1);
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		printf("listen() Error");
		exit(1);
	}

	clntAddrSize = sizeof(clntAddr);

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
	if (hClntSock == INVALID_SOCKET)
	{
		printf("accept() Error");
		exit(1);
	}
	else
	{
		printf("Connected\n");
	}

	int msgLen = 0;
	char buf[BUF_SIZE - 4] = {};
	char msg[BUF_SIZE] = {};
	char clntMsg[BUF_SIZE] = {};

	for (int i = 0; i < 3; ++i)
	{
		sprintf_s(buf, "This is Message From Server %d.", i);

		msgLen = strlen(buf);

		memcpy(msg, &msgLen, 4);
		memcpy(&msg[4], buf, msgLen);

		if (send(hClntSock, msg, msgLen + sizeof(int), 0) == -1)
		{
			printf("send() Error.");
			break;
		}

		int recvCnt = 0;
		int recvLen = 0;

		while (recvLen < sizeof(int))
		{
			recvCnt = recv(hClntSock, &clntMsg[recvLen], sizeof(int), 0);

			if (recvCnt == -1)
			{
				printf("recv() Error.");
				break;
			}

			recvLen += recvCnt;
		}

		int clntMsgLen = clntMsg[0];

		recvCnt = 0;
		recvLen = 0;

		while (recvLen < clntMsgLen)
		{
			recvCnt = recv(hClntSock, &clntMsg[recvLen], BUF_SIZE - 4, 0);

			if (recvCnt == -1)
			{
				printf("recv() Error.");
				break;
			}

			recvLen += recvCnt;
		}

		clntMsg[recvLen] = '\0';
		printf("Message From Client : %s\n", clntMsg);
	}

	closesocket(hClntSock);
	closesocket(hServSock);

	printf("Close Server.\n");
	WSACleanup();
}
