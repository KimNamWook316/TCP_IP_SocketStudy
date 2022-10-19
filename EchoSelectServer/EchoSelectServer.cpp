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

	// 데이터 수신 여부를 관찰하는 관찰 대상 목록 fd_set에 서버 소켓을 등록한다.
	// 클라이언트의 연결 요청도 데이터 전송을 통해 이루어진다. (SYN)
	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);

	while (true)
	{
		// select 함수가 반환되고 나면, 인자로 넘겨진 fd_set은 이벤트가 발생하지 않은 소켓의 비트는 0으로 초기화한다. 
		// 관찰 대상을 관리하기 위해 임시 변수에 복사해서 사용한다.
		cpyReads = reads;

		// timeout또한 반환되고 나면 timeout되기 전까지 남은 시간을 반환하기 때문에
		// 루프마다 초기화해주어야 한다.
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
		{
			break;
		}

		// 아무 이벤트가 없는 경우
		if (fdNum == 0)
			continue;

		// 관찰 대상 목록을 순회하며 이벤트 처리
		for (i = 0; i < reads.fd_count; ++i)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyReads))
			{
				// 데이터 수신 이벤트가 일어난 핸들이 서버일 경우 : 클라이언트의 연결 요청 
				if (reads.fd_array[i] == hServSock)
				{
					adrSz = sizeof(clntAddr);
					hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &adrSz);

					// 데이터 수신 관찰 목록에 클라이언트 소켓 핸들 추가
					FD_SET(hClntSock, &reads);
					printf("Connected Client : %d\n", hClntSock);
				}

				// 서버 소켓 핸들이 아닌 경우 : 클라이언트로부터의 메시지 수신
				else
				{
					strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
					if (strLen == 0)
					{
						// 연결 종료된 경우, 관찰 목록에서 해당 클라이언트 소켓 핸들을 삭제
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
