#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<winsock.h>

#define SOCKET_DEPRECATED_NO_WARNING

#include"ConnectionControl.h"
#include"ClientFunctions.h"

SOCKET controlConnectSocket;


int main(int argc, char* argv[])
{
	if (!InitializeConnection(&controlConnectSocket))
	{
		getch();
		return 1;
	}
	while(!Login(&controlConnectSocket));

	int option;
	do
	{
		printf("select from 1 to 6\n");
		printf(	"1.Display files in home directory\n"
				"2.Download\n"
				"3.Upload\n"
				"4.Rename\n"
				"5.Remove\n"
				"6.Quit\n");
		fflush(stdin);
		scanf("%d", &option);
		switch (option)
		{
		case 1:
			Display(&controlConnectSocket);
			break;
		case 2:
			Download(&controlConnectSocket);
			break;
		case 3:
			Upload(&controlConnectSocket);
			break;
		case 4:
			Rename(&controlConnectSocket);
			break;
		case 5:
			Remove(&controlConnectSocket);
			break;
		default:
			break;
		}
	} while (option != 6);

	CloseConnection(controlConnectSocket);
	return 0;
}