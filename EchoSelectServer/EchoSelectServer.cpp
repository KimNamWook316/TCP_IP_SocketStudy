#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;
	TIMEVAL timeout;
	fd_set reads, cpyReads;

	int adrSz;
	int strLen, fdNum, i;
	char buf[BUF_SIZE] = {};

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartUp() Error");
	}

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() Error");
	}

	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() Error");
	}

	// ������ ���� ���θ� �����ϴ� ���� ��� ��� fd_set�� ���� ������ ����Ѵ�.
	// Ŭ���̾�Ʈ�� ���� ��û�� ������ ������ ���� �̷������. (SYN)
	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);

	while (true)
	{
		// select �Լ��� ��ȯ�ǰ� ����, ���ڷ� �Ѱ��� fd_set�� �̺�Ʈ�� �߻����� ���� ������ ��Ʈ�� 0���� �ʱ�ȭ�Ѵ�. 
		// ���� ����� �����ϱ� ���� �ӽ� ������ �����ؼ� ����Ѵ�.
		cpyReads = reads;

		// timeout���� ��ȯ�ǰ� ���� timeout�Ǳ� ������ ���� �ð��� ��ȯ�ϱ� ������
		// �������� �ʱ�ȭ���־�� �Ѵ�.
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
		{
			break;
		}

		// �ƹ� �̺�Ʈ�� ���� ���
		if (fdNum == 0)
			continue;

		// ���� ��� ����� ��ȸ�ϸ� �̺�Ʈ ó��
		for (i = 0; i < reads.fd_count; ++i)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				// ������ ���� �̺�Ʈ�� �Ͼ �ڵ��� ������ ��� : Ŭ���̾�Ʈ�� ���� ��û 
				if (reads.fd_array[i] == hServSock)
				{
					adrSz = sizeof(clntAddr);
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &adrSz);

					// ������ ���� ���� ��Ͽ� Ŭ���̾�Ʈ ���� �ڵ� �߰�
					FD_SET(hClntSock, &reads);
					printf("Connected Client : %d\n", hClntSock);
				}

				// ���� ���� �ڵ��� �ƴ� ��� : Ŭ���̾�Ʈ�κ����� �޽��� ����
				else
				{
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
					if (strLen == 0)
					{
						// ���� ����� ���, ���� ��Ͽ��� �ش� Ŭ���̾�Ʈ ���� �ڵ��� ����
						FD_CLR(reads.fd_array[i], &reads);
						closesocket(cpyReads.fd_array[i]);
						printf("Closed Client %d\n", cpyReads.fd_array[i]);
					}
					else
					{
						// Echo
						send(reads.fd_array[i], buf, strLen, 0);
					}
				}
			}
		}
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputs("\n", stderr);
	exit(1);
}
