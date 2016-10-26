#include<cstdlib>
#include<WinSock2.h>
#include"ClientFunctions.h"

bool Login(SOCKET* controlConnectSocket)
{
	char username[30];
	char password[30];
	char userCommand[35];
	char passCommand[35];
	char recvBuffer[512];
	int recvBytes;
	
	while (true)
	{
		printf("Username: ");
		scanf("%s", &username);

		strcpy(userCommand, "user ");
		strcat(userCommand, username);
		strcat(userCommand, "\n");

		send(*controlConnectSocket, userCommand, strlen(userCommand), 0);

		recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
		if (recvBytes > 0)
		{
			recvBuffer[recvBytes] = '\0';
			if (strncmp(recvBuffer, "331", 3)==0)// hoan thanh lenh USER <username>
			{
				printf("Password: ");
				scanf("%s", &password);

				strcpy(passCommand, "pass ");
				strcat(passCommand, password);
				strcat(passCommand, "\n");

				send(*controlConnectSocket, passCommand, strlen(passCommand), 0);

				recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
				if (recvBytes > 0)
				{
					recvBuffer[recvBytes] = '\0';
					if (strncmp(recvBuffer, "230", 3)==0)
					{
						break;//--> dang nhap thanh cong
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}
void EstablishDataChannel(SOCKET* controlConnectSocket,SOCKET* dataSocket)
{
	char pasvCommand[4];
	char recvBuffer[200];
	int recvBytes;

	strcpy(pasvCommand, "pasv");
	send(*controlConnectSocket, pasvCommand, 5, 0);

	recvBytes = recv(*controlConnectSocket, recvBuffer, 200, 0);
	recvBuffer[recvBytes] = '\0';

	int dataPort = GetPortNumber(recvBuffer);

	SOCKADDR_IN serverAddress;//dia chi server voi dataport
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dataPort);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(*dataSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
	{
		printf("cannot establish data channel, error : %d\n", GetLastError());
		return;
	}

}
int GetPortNumber(char recvBuffer[200])
{
	//227 entering passive mode (h1,h2,h3,h4,p1,p2)
	
	char* address;
	char* ptr;
	int result[6];

	address = &recvBuffer[27];

	ptr = strtok(address, ",)");

	for (int i = 0; i < 6; i++)
	{
		result[i] = atoi(ptr);
		ptr = strtok(NULL, ",)");
	}
	return result[4] * 256 + result[5];// dataport = p1*256 + p2
}
int Display(SOCKET* controlConnectSocket,SOCKET* dataSocket)
{
	char pasvCommand[5];
	char listCommand[10];
	char recvBuffer[200];
	int recvBytes;
	
	//strcpy(pasvCommand, "pasv");
	//send(*controlConnectSocket, pasvCommand, 5, 0);

	//recvBytes = recv(*controlConnectSocket, recvBuffer, 200, 0);
	//recvBuffer[recvBytes] = '\0';

	//int dataPort = GetPortNumber(recvBuffer);

	//SOCKADDR_IN serverAddress;//dia chi server voi dataport
	//serverAddress.sin_family = AF_INET;
	//serverAddress.sin_port = htons(dataPort);
	//serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	//if (connect(*dataSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
	//{
	//	printf("cannot establish data channel, error : %d\n", GetLastError());
	//	return 1;
	//}
	EstablishDataChannel(controlConnectSocket, dataSocket);

	strcpy(listCommand, "LIST\n");
	send(*controlConnectSocket, listCommand, strlen(listCommand), 0);

	char dataBuffer[2048];
	recvBytes = recv(*dataSocket, dataBuffer, 2048, 0);
	dataBuffer[recvBytes] = '\0';

	system("cls");
	printf("%s\n", dataBuffer);
	closesocket(*dataSocket);
	shutdown(*dataSocket, SD_RECEIVE);
	return 0;
}
void Download() 
{

}
void Upload() 
{

}
void Rename() 
{

}
void Remove() 
{

}
