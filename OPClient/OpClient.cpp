#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

void main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;

	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Error WSAStartUP()\n");
		exit(1);
	}

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
	{
		printf("Error socket()\n");
		exit(1);
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	int errConnect = connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr));

	if (errConnect == SOCKET_ERROR)
	{
		printf("Error socket()\n");
		exit(1);
	}
	else
	{
		printf("Connected OP Client Server\n");
	}

	char op;
	int numArr[3] = {};

	for (int i = 0; i < 3; ++i)
	{
		printf("%d 번째 숫자를 입력 : ", i + 1);
		scanf_s("%d", &numArr[i]);
	}
	printf("\n");

	printf("연산자를 입력 : '+', '-', '*' : ");
	getchar();
	scanf_s("%c", &op);
	printf("\n");

	send(hSocket, &op, sizeof(char), 0);
	send(hSocket, (char*)numArr, sizeof(numArr), 0);

	int receive = 0;
	int recvSize = 0;
	int recvCount = 0;

	while (recvSize < sizeof(int))
	{
		recvCount = recv(hSocket, (char*)&receive, sizeof(int), 0);

		if (recvCount == -1)
		{
			printf("recv() Error\n");
		}

		recvSize += recvCount;
	}

	printf("서버로부터 계산된 결과 : %d", receive);

	closesocket(hSocket);
	WSACleanup();

	return;
}
