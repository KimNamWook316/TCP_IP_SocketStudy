#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void main(int argc, char* argv[])
{
	WSADATA wsaData = {};
	SOCKET hSocket;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartUp() Error");
		exit(1);
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
	{
		printf("socket() Error");
		exit(1);
	}

	SOCKADDR_IN servAddr = {};
	memset(&servAddr, 0, sizeof(servAddr));
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
		printf("Connected..................\n");
	}

	char message[BUF_SIZE] = {};
	int recvLen = 0;
	while (true)
	{
		fputs("Input Message (Q to Quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}

		send(hSocket, message, strlen(message), 0);

		recvLen = recv(hSocket, &message[0], BUF_SIZE - 1, 0);
		message[recvLen] = '\0';
		printf("Message From Server : %s", message);
	}

	closesocket(hSocket);
	printf("Quit Client.\n");
}