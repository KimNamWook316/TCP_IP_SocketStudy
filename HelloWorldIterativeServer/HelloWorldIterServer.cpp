#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define QUEUE_SIZE 5
#define BUF_SIZE 1024

const char* serverMessage = "Hello World!\n";

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

	// WSA library 2.2 �������� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartUp() Error");
		exit(1);
	}

	// IPv4, TCP ���� ����
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
	{
		printf("socket() Error");
		exit(1);
	}

	// �ּ� ���� �ʱ�ȭ
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET; // IPv4����� �ּ� ü�� ���
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // ���� ��ǻ���� IP�ּҸ� IP�ּҷ� ���, host to Network Byte Order
	servAddr.sin_port = htons(atoi(argv[1]));

	// IP, PORT �Ҵ�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind() Error");
		exit(1);
	}

	// ���� ��û ������ ���� (���� ��û ��� ť size : 5)
	if (listen(hServSock, QUEUE_SIZE) == SOCKET_ERROR)
	{
		printf("listen() Error");
		exit(1);
	}

	clntAddrSize = sizeof(clntAddr);

	int clntStrLen = 0;
	char clntMessage[BUF_SIZE] = {};

	// �ִ� 5���� Ŭ���̾�Ʈ ��û���� ó��
	for (int i = 0; i < QUEUE_SIZE; ++i)
	{
		// ���� ��û ����
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);

		if (hClntSock == INVALID_SOCKET)
		{
			printf("accept() Error");
			exit(1);
		}
		else
		{
			printf("Client [%d] Connected.\n", i + 1);
		}

		do
		{
			// ���� �����Ͱ� ���� ������ ���
			clntStrLen = recv(hClntSock, clntMessage, BUF_SIZE, 0);

			if (clntStrLen == 0)
				break;

			// Hello World �۽�
			send(hClntSock, serverMessage, strlen(serverMessage), 0);
		} while (clntStrLen != 0);

		closesocket(hClntSock);
	}

	closesocket(hServSock);

	printf("Close Server.\n");
	WSACleanup();
}

