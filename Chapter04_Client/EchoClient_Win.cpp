#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void ErrorHandling(const char* Message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE] = {};
	int strLen = 0;
	int recvLen = 0;
	int recvCount = 0;
	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartUp() Error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() Error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() Error");
	}
	else
	{
		printf("Connected............");
	}

	while (true)
	{
		fputs("Input Message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}

		strLen = send(hSocket, message, strlen(message), 0);

		recvLen = 0;
		while (recvLen < strLen)
		{
			recvCount = recv(hSocket, &message[recvLen], BUF_SIZE - 1, 0);
			if (recvCount == -1)
			{
				ErrorHandling("recv() Error");
			}
			recvLen += recvCount;
		}

		message[recvLen] = '\0';
		printf("Message from server : %s", message);
	}
	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* Message)
{
	fputs(Message, stderr);
	fputc('\n', stderr);
	exit(1);
}
