#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock;
	SOCKET hClntSock = 0;

	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddrSize;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Error WSAStartUP()\n");
		exit(1);
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	
	if (hServSock == INVALID_SOCKET)
	{
		printf("Error socket()\n");
		exit(1);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("Error bind()\n");
		exit(1);
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		printf("listen() Error()\n");
		exit(1);
	}

	clntAddrSize = sizeof(clntAddr);

	int receiveSize = 0;
	char op;
	int numArr[3] = {};

	for (int i = 0; i < 5; ++i)
	{
		op = 0;
		for (int i = 0; i < 3; ++i) numArr[i] = 0;

		clntAddrSize = sizeof(clntAddr);

		hClntSock = accept(hServSock, (SOCKADDR*)hClntSock, &clntAddrSize);

		if (hClntSock == INVALID_SOCKET)
		{
			printf("accept() Error()\n");
			exit(1);
		}
		else
		{
			printf("Connected Client %d\n", i);
		}

		receiveSize = recv(hClntSock, &op, sizeof(char), 0);

		if (receiveSize == 0)
		{
			printf("recv() Error\n");
			exit(1);
		}

		printf("Operator By Client : %c\n", op);

		int numCount = 0;
		while (numCount < 3)
		{
			receiveSize = recv(hClntSock, (char*)&numArr[numCount], sizeof(int), 0);

			printf("Number By Client : %d\n", numArr[numCount]);

			if (receiveSize == 0)
			{
				printf("recv() Error\n");
				exit(1);
			}
			else
			{
				++numCount;
			}
		}

		int calc = 0;
		for (int i = 0; i < 3; ++i)
		{
			if (op == '+')
				calc += numArr[i];
			else if (op == '-')
				calc -= numArr[i];
			else if (op == '*')
				calc *= numArr[i];
		}

		send(hClntSock, (char*)&calc, sizeof(int), 0);
		closesocket(hClntSock);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}
