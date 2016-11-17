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

		//USER username
		sprintf(userCommand, "USER %s\n", username);

		send(*controlConnectSocket, userCommand, strlen(userCommand), 0);

		recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
		if (recvBytes > 0)
		{
			recvBuffer[recvBytes] = '\0';
			// hoan thanh lenh USER <username>
			if (strncmp(recvBuffer, "331", 3)==0)
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
						//--> dang nhap thanh cong
						break;
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
	// dataport = p1*256 + p2
	return port;
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
	//
	//Mo kenh data 
	EstablishDataChannel(controlConnectSocket,dataSocket);
	strcpy(listCommand, "LIST\n");
	send(*controlConnectSocket, listCommand, strlen(listCommand), 0);

	recvBytes = recv(*controlConnectSocket, recvBuffer, 200, 0);
	recvBytes = recv(dataSocket, dataBuffer, 2048, 0);
	dataBuffer[recvBytes] = '\0';
	system("cls");

	printf("%s\n", dataBuffer);
	closesocket(dataSocket);
}
void RegularDownload(SOCKET* controlConnectSocket)
{
	char* filename;
	char retrCommand[100];
	char recvBuffer[4096];
	int recvBytes;
	FILE* file;
	Display(controlConnectSocket);
	printf("Type the file's name you want to download: ", &filename);

	SOCKET dataSocket;
	dataSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	EstablishDataChannel(controlConnectSocket, dataSocket);

	//RETR <filename>
	strcpy(retrCommand, "RETR ");
	strcat(retrCommand, filename);
	strcat(retrCommand, "\n");

	send(*controlConnectSocket, retrCommand, strlen(retrCommand), 0);

	//can phai lay them byte size de cap phat dong cho recvBuffer
	recvBytes = recv(dataSocket, recvBuffer, 4096, 0);

	file = fopen(filename, "wb");
	if (file == NULL)
	{
		printf("Error creating file\n");
		return;
	}
	else
	{
		fprintf(file, "%d", recvBuffer);
	}
	fclose(file);
	printf("File downloaded !\n");
	closesocket(dataSocket);
}
void MultiConnectsDownload(SOCKET* controlConnectSocket)
{

}
void Download(SOCKET* controlConnectSocket) 
{
	//hien thi cac tep trong thu muc cua server
	//yeu cau nguoi dung go ten tep 

	//
	//thiet lap kenh truyen 
	//
	//luu tep vao thu muc chua chuong trinh 
	char option;
	do 
	{
		printf(	"Select download method\n"
				"1.Regular download\n"
				"2.Multi-connects download\n");
		scanf("%c", &option);
	} while (option < 1|| option > 2);
	switch (option)
	{
		case 1:
			RegularDownload(controlConnectSocket);
			break;
		case 2:
			MultiConnectsDownload(controlConnectSocket);
			break;
	}
}
void Upload(SOCKET* controlConnectSocket) 
{
	//Hien thi cac tep cua nguoi dung 
	//yeu cau nguoi dung go ten tep 
	//thiet lap kenh truyen 
	//tai len server
	//nhan thong diep thao tac tu server
	char* filename;
	char storCommand[100];
	char recvByte[100];
	char sendBuffer[1024];
	int recvBytes;
	int filesize;
	int sentBytes = 0;

	//liet ke danh sach file trong tep cua nguoi dung
	system("cd data");
	system("dir");
	printf("Type the file's name you want to upload\n");
	scanf("%s", &filename);

	FILE* uploadFile;
	uploadFile = fopen(filename, "rb");
	//tim filesize
	fseek(uploadFile, 0L, SEEK_END);
	filesize = ftell(uploadFile);
	rewind(uploadFile);
	//mo datasocket
	SOCKET dataSocket;
	dataSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	EstablishDataChannel(controlConnectSocket, dataSocket);
	//gui lenh
	//STOR <filename>
	sprintf(storCommand, "STOR %s\n", filename);
	while (sentBytes < filesize)
	{
		int read = fread(sendBuffer, 1, 1024, uploadFile);
		if (read < 0)
		{
			break;
		}
		send(dataSocket, sendBuffer, 1024, 0);
		sentBytes += 1024;
	}
	fclose(uploadFile);
	closesocket(dataSocket);
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
	sprintf(rnfrCommand, "RNFR %s\n", oldName);
	//RNTO <filename>
	sprintf(rntoCommand, "RNTO %s\n", newName);

	send(*controlConnectSocket, rnfrCommand, strlen(rnfrCommand), 0);
	recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
	recvBuffer[recvBytes] = '\0';
	//nhan ket qua tra ve tu server
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
	sprintf(deleteCommand, "DELE %s\n", filename);

	send(*controlConnectSocket, deleteCommand, strlen(deleteCommand), 0);
	recvBytes = recv(*controlConnectSocket, recvBuffer, 512, 0);
	recvBuffer[recvBytes] = '\0';
	//nhan ket qua tu server 
	if (strncmp(recvBuffer, "", 3) == 0)
	{

	}
	printf("Deleting file succeeded\n");

}
