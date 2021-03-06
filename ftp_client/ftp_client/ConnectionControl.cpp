#include"ConnectionControl.h"
#include"ClientFunctions.h"

bool InitializeConnection(SOCKET* controlConnectSocket)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("WSAStartup error %d\n", GetLastError());
		return false;
	}
	//socket dieu khien ket noi
	*controlConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(21);//control-connection port
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (!connect(*controlConnectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
	{
		char buffer[512];
		int recvBytes = recv(*controlConnectSocket, buffer, 512, 0);
		buffer[recvBytes] = '\0';
		printf("%s\n", buffer);
	}
	else
	{
		printf("Cannot connect to server!\n");
		return false;
	}
	return true;
}
void CloseConnection(SOCKET controlConnectSocket)
{
	closesocket(controlConnectSocket);
	WSACleanup();
}