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

	// WSA library 2.2 버전으로 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartUp() Error");
		exit(1);
	}

	// IPv4, TCP 소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hServSock == INVALID_SOCKET)
	{
		printf("socket() Error");
		exit(1);
	}

	// 주소 정보 초기화
	memset(&servAddr, 0, sizeof(SOCKADDR_IN));
	servAddr.sin_family = AF_INET; // IPv4기반의 주소 체계 사용
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 현재 컴퓨터의 IP주소를 IP주소로 사용, host to Network Byte Order
	servAddr.sin_port = htons(atoi(argv[1]));

	// IP, PORT 할당
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("bind() Error");
		exit(1);
	}

	// 연결 요청 대기상태 진입 (연결 요청 대기 큐 size : 5)
	if (listen(hServSock, QUEUE_SIZE) == SOCKET_ERROR)
	{
		printf("listen() Error");
		exit(1);
	}

	clntAddrSize = sizeof(clntAddr);

	int clntStrLen = 0;
	char clntMessage[BUF_SIZE] = {};

	// 최대 5개의 클라이언트 요청까지 처리
	for (int i = 0; i < QUEUE_SIZE; ++i)
	{
		// 연결 요청 수락
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
			// 수신 데이터가 있을 때까지 대기
			clntStrLen = recv(hClntSock, clntMessage, BUF_SIZE, 0);

			if (clntStrLen == 0)
				break;

			// Hello World 송신
			send(hClntSock, serverMessage, strlen(serverMessage), 0);
		} while (clntStrLen != 0);

		closesocket(hClntSock);
	}

	closesocket(hServSock);

	printf("Close Server.\n");
	WSACleanup();
}

