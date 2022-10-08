 //#include <stdio.h>
 //#include <stdlib.h>
 //#include <WinSock2.h>
 //
 //#pragma comment(lib, "ws2_32.lib")
 //
 //void ErrorHandling(char* Message);
 //
 //int main(int argc, char* argv[])
 //{
 //	WSADATA wsaData;
 //	SOCKET hServerSock, hClntSock;
 //	SOCKADDR_IN servAddr, clntAddr;
 //
 //	int szClntAddr;
 //	char message[] = "Hello World!";
 //
 //	if (argc != 2)
 //	{
 //		printf("Usage %s <port>\n", argv[0]);
 //		exit(1);
 //	}
 //
 //	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
 //	{
 //		ErrorHandling("WSAStartUp() error");
 //	}
 //
 //	hServerSock = socket(PF_INET, SOCK_STREAM, 0);
 //	if (hServerSock == INVALID_SOCKET)
 //	{
 //		ErrorHandling("socket() error");
 //	}
 //
 //	memset(&servAddr, 0, sizeof(servAddr));
 //	servAddr.sin_family = AF_INET;
 //	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 //	servAddr.sin_port = htons(atoi(argv[1]));
 //
 //	if (bind(hServerSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
 //	{
 //		ErrorHandling("bind() error");
 //	}
 //
 //	if (listen(hServerSock, 5) == SOCKET_ERROR)
 //	{
 //		ErrorHandling("listen() error");
 //	}
 //
 //	szClntAddr = sizeof(clntAddr);
 //	hClntSock = accept(hServerSock, (SOCKADDR*)&clntAddr, &szClntAddr);
 //
 //	if (hClntSock == INVALID_SOCKET)
 //	{
 //		ErrorHandling("accept() error");
 //	}
 //
 //	send(hClntSock, message, sizeof(message), 0);
 //	closesocket(hClntSock);
 //	closesocket(hServerSock);
 //	WSACleanup();
 //}
 //
 //void ErrorHandling(char* Message)
 //{
 //	fputs(Message, stderr);
 //	fputc('\n', stderr);
 //	exit(1);
 //}
