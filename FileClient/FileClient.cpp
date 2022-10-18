
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

void main(int argc, char* argv[])
{
	for (int i = 0; i < argc; ++i)
	{
		printf("argv[%d] : %s\n", i, argv[i]);
	}
	if (argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	WSADATA wsaData = {};
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartUp() Error");
		exit(1);
	}

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
	{
		printf("socket() Error");
		exit(1);
	}

	SOCKADDR_IN	servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
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
		printf("Connected.........\n");
	}


	// ���ϸ� �Է�
	char fileName[MAX_PATH] = {};
	memset(fileName, 0, MAX_PATH);
	printf("���۹��� ���ϸ� : ");
	fgets(fileName, MAX_PATH, stdin);

	// ���ϸ� ���� ����, ���๮�� ����
	char buf[4] = {};
	int len = strlen(fileName) - 1;

	for (int i = 0; i < 4; ++i)
	{
		buf[i] = len >> (3 - i) * 8;
	}

	send(hSocket, buf, 4, 0);

	// ���ϸ� ����
	send(hSocket, fileName, len, 0);

	// ���� ������ �޴´�.
	int recvCnt = 0;
	int recvLen = 0;

	while (recvLen < 4)
	{
		recvCnt = recv(hSocket, &buf[recvLen], sizeof(int), 0);

		if (recvCnt == -1)
		{
			printf("recv() Error\n");
			closesocket(hSocket);
			return;
		}

		recvLen += recvCnt;
	}

	int fileSize = 0;
	for (int i = 3; i >= 0; --i)
	{
		fileSize += buf[i] >> (8 * (3 - i));
	}

	recvCnt = 0;
	recvLen = 0;

	// ���� �ʱ�ȭ
	char* fileBuf = (char*)malloc(fileSize);
	memset(fileBuf, 0, fileSize);

	// ���� ���۹޴´�.
	while (recvLen < fileSize)
	{
		recvCnt = recv(hSocket, &fileBuf[recvLen], fileSize, 0);

		if (recvCnt == -1)
		{
			printf("recv() Error\n");
			closesocket(hSocket);
			return;
		}

		recvLen += recvCnt;
	}

	// ���������� ������ ���۹޾Ҵٸ�, ���ÿ� ����.
	FILE* fp = nullptr;
	fileName[len] = '\0';
	printf("fileName : %s\n", fileName);
	errno_t err = fopen_s(&fp, fileName, "w");

	if (!fp)
	{
		printf("Error : %d\n", err);
		printf("fopen_s() Error\n");
		closesocket(hSocket);
		return;
	}

	fwrite(fileBuf, sizeof(char), fileSize, fp);

	printf("���� ���� �Ϸ�\n");

	fclose(fp);
	closesocket(hSocket);
	WSACleanup();
}
