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
	SOCKET hServSock;
	SOCKET hClntSock;
	char message[BUF_SIZE] = {};
	int strLen = 0;
	int i = 0;

	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddrSz;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// ���̺귯�� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartUp() Error");
	}

	// ���� ���� (IPv4, TCP)
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() Error");
	}

	memset(&servAddr, 0, sizeof(servAddr));

	// �������� ���� �� PORT��ȣ ����
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	// IP�ּ� �� PORT�Ҵ�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() Error");
	}

	// ���� ������, �����û ��� ť ������ 5�� ����
	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() Error");
	}

	clntAddrSz = sizeof(clntAddr);

	// 5���� Ŭ���̾�Ʈ ��û ó��
	for (int i = 0; i < 5; ++i)
	{
		// ���� ��û ����
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSz);

		if (hClntSock == -1)
		{
			ErrorHandling("accept() Error");
		}
		else
		{
			printf("Connected Client %d \n", i);
		}

		// Client�� ���� ������ ������ recv�� 0�� ��ȯ,
		// ���� ����� ������ echo
		while ((strLen = recv(hClntSock, message, BUF_SIZE, 0)) != 0)
		{
			send(hClntSock, message, strLen, 0);
		}

		closesocket(hClntSock);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* Message)
{
	fputs(Message, stderr);
	fputc('\n', stderr);
	exit(1);
}
