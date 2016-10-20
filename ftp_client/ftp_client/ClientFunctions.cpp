#include"ClientFunctions.h"

bool Login(SOCKET controlConnectSocket)
{
	char username[30];
	char password[30];

	printf("Username: \n");
	scanf("%s", &username);
	printf("Password: \n");
	scanf("%s", &password);

	char command[35];
	strcat(command, "user ");

	return true;

}
void Display(SOCKET controlConnectSocket) 
{

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