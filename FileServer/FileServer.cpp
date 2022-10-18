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

	printf("Accept 대기중...\n");

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

	// 경로 길이
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

	// 경로 받는다.
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

	// 파일이 없는 경우 연결 종료
	if (!fp)
	{
		printf("No File");
		closesocket(hServSock);
		closesocket(hClntSock);
		return;
	}

	// 파일 사이즈
	fseek(fp, 0, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// 버퍼 초기화
	char* fileBuf = (char*)malloc(fileSize);
	memset(fileBuf, 0, fileSize);

	// 파일 읽기
	fread(fileBuf, sizeof(char), fileSize, fp);

	// 파일 사이즈부터 보낸다.
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

	// 파일 내용을 전송
	if (send(hClntSock, fileBuf, fileSize, 0) == -1)
	{
		printf("recv() Error");
		free(fileBuf);
		fclose(fp);
		closesocket(hServSock);
		closesocket(hClntSock);
		return;
	}

	printf("파일 전송 완료\n");

	// 버퍼 해제 및 초기화
	free(fileBuf);
	WSACleanup();
	fclose(fp);
	closesocket(hServSock);
	closesocket(hClntSock);
}

