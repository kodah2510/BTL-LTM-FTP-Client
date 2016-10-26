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
int GetPortNumber(char recvBuffer[200])
{
	//227 entering passive mode (h1,h2,h3,h4,p1,p2)

	char* address;
	char* ptr;
	int result[6];
	int port;
	address = &recvBuffer[27];

	ptr = strtok(address, ",)");

	for (int i = 0; i < 6; i++)
	{
		result[i] = atoi(ptr);
		ptr = strtok(NULL, ",)");
	}
	port = result[4] * 256 + result[5];


	return port;// dataport = p1*256 + p2
}
bool EstablishDataChannel(SOCKET* controlConnectSocket,SOCKET dataSocket)
{
	char pasvCommand[5];
	char recvBuffer[200];
	int recvBytes;

	strcpy(pasvCommand, "pasv");
	send(*controlConnectSocket, pasvCommand, 5, 0);

	recvBytes = recv(*controlConnectSocket, recvBuffer, 200, 0);
	recvBuffer[recvBytes] = '\0';

	if (strncmp(recvBuffer, "421", 3) == 0)
	{
		printf("server cannot create socket error 421\n");
		return false;
	}

	int dataPort = GetPortNumber(recvBuffer);

	SOCKADDR_IN serverAddress;//dia chi server voi dataport
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(dataPort);
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (connect(dataSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)))
	{
		printf("cannot establish data channel, error : %d\n", GetLastError());
		return false;
	}
	return true;
}

void Display(SOCKET* controlConnectSocket)
{
	char pasvCommand[5];
	char listCommand[10];
	char recvBuffer[200];
	char dataBuffer[2048];
	int recvBytes;

	SOCKET dataSocket;
	dataSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	EstablishDataChannel(controlConnectSocket,dataSocket);
	strcpy(listCommand, "LIST\n");
	send(*controlConnectSocket, listCommand, strlen(listCommand), 0);

	recvBytes = recv(*controlConnectSocket, recvBuffer, 200, 0);
	recvBytes = recv(dataSocket, dataBuffer, 2048, 0);
	dataBuffer[recvBytes] = '\0';
	system("cls");
	printf("%s\n", dataBuffer);
}
void Download() 
{

}
void Upload() 
{

}
void Rename(SOCKET* controlConnectSocket) 
{
	char rnfrCommand[30];
	char rntoCommand[30];
	char oldName[20];
	char newName[20];

	char recvBuffer[512];
	int recvBytes;
	
	system("cls");
	printf("Type a filename you want to rename \n");
	scanf("%s", &oldName);

	printf("Type a new filename: \n");
	scanf("%s", &newName);

	//RNFR <filename>
	strcpy(rnfrCommand, "rnfr ");
	strcat(rnfrCommand, oldName);
	strcat(rnfrCommand, "\n");
	//RNTO <filename>
	strcat(rntoCommand, "rnto ");
	strcat(rntoCommand, newName);
	strcat(rntoCommand, "\n");

	send(*controlConnectSocket, rnfrCommand, strlen(rnfrCommand), 0);
	recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
	recvBuffer[recvBytes] = '\0';
	if (strncmp(recvBuffer, " ", 3) == 0)
	{

	}

	send(*controlConnectSocket, rntoCommand, strlen(rntoCommand), 0);
	recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);

	printf("Renaming file succeeded\n");

}
void Remove(SOCKET* controlConnectSocket) 
{
	char deleteCommand[20];
	char filename[30];

	char recvBuffer[512];
	int recvBytes;
	system("cls");
	printf("Type the filename you want to delete: \n");
	scanf("%s", &filename);

	//DELE <filename>
	strcpy(deleteCommand, "dele ");
	strcat(deleteCommand, filename);
	strcat(deleteCommand, "\n");

	send(*controlConnectSocket, deleteCommand, strlen(deleteCommand), 0);
	recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
	recvBuffer[recvBytes] = '\0';

	if (strncmp(recvBuffer, "", 3) == 0)
	{

	}
	printf("Deleting file succeeded\n");

}
