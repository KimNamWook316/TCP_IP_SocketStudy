#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

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

	if (listen(hServSock, 1) == SOCKET_ERROR)
	{
		printf("listen() Error");
		exit(1);
	}

	clntAddrSize = sizeof(clntAddr);

	printf("Accept �����...\n");

	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &clntAddrSize);
	if (hClntSock == INVALID_SOCKET)
	{
		printf("accept() Error");
		exit(1);
	}
	else
	{
		printf("Connected...\n");
	}

	int recvCnt = 0, recvLen = 0;
	char buf[MAX_PATH] = {};

	// ��� ����
	while (recvLen < 4)
	{
		recvCnt = recv(hClntSock, &buf[recvLen], sizeof(int), 0);

		if (recvCnt == -1)
		{
			printf("recv() Error");
			closesocket(hServSock);
			closesocket(hClntSock);
			return;
		}

		recvLen += recvCnt;
	}

	int pathLen = 0;

	for (int i = 3; i >= 0; --i)
	{
		pathLen += buf[i] >> (8 * (3 - i));
	}

	recvCnt = 0;
	recvLen = 0;

	// ��� �޴´�.
	while (recvCnt < pathLen)
	{
		recvCnt = recv(hClntSock, &buf[recvLen], MAX_PATH, 0);

		if (recvCnt == -1)
		{
			printf("recv() Error");
			closesocket(hServSock);
			closesocket(hClntSock);
			return;
		}

		recvLen += recvCnt;
	}


	FILE* fp = nullptr;
	fopen_s(&fp, buf, "r");

	// ������ ���� ��� ���� ����
	if (!fp)
	{
		printf("No File");
		closesocket(hServSock);
		closesocket(hClntSock);
		return;
	}

	// ���� ������
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ���� �ʱ�ȭ
	char* fileBuf = (char*)malloc(fileSize);
	memset(fileBuf, 0, fileSize);

	// ���� �б�
	fread(fileBuf, sizeof(char), fileSize, fp);

	// ���� ��������� ������.
	for (int i = 0; i < 4; ++i)
	{
		buf[i] = fileSize >> (3 - i) * 8;
	}

	if (send(hClntSock, buf, sizeof(int), 0) == -1)
	{
		printf("recv() Error");
		free(fileBuf);
		fclose(fp);
		closesocket(hServSock);
		closesocket(hClntSock);
		return;
	}

	// ���� ������ ����
	if (send(hClntSock, fileBuf, fileSize, 0) == -1)
	{
		printf("recv() Error");
		free(fileBuf);
		fclose(fp);
		closesocket(hServSock);
		closesocket(hClntSock);
		return;
	}

	printf("���� ���� �Ϸ�\n");

	// ���� ���� �� �ʱ�ȭ
	free(fileBuf);
	WSACleanup();
	fclose(fp);
	closesocket(hServSock);
	closesocket(hClntSock);
}

