#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(char* Message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char message[30];

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error");
	}

	// busy - waiting
	for (int i = 0; i < 3000; ++i)
	{
		printf("Wait Time : %d\n", i);
	}

	recv(hSocket, message, 12, 0);
 //	int readLen = 0;
 //	int strLen = 0;
 //	int idx = 0;
 //
 //	while (readLen = recv(hSocket, &message[idx++], 1, 0))
 //	{
 //		if (readLen == -1)
 //		{
 //			ErrorHandling("read() error");
 //		}
 //
 //		strLen += readLen;
 //	}

	printf("Message From Server : %s", message);
	// printf("Function Read Call Count : %d\n", strLen);

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(char* Message)
{
	fputs(Message, stderr);
	fputc('\n', stderr);
	exit(1);
}
