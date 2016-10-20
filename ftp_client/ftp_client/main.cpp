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
	if (!InitializeConnection(controlConnectSocket))
	{
		getch();
		return 1;
	}
	char option;
	do
	{
		printf("select from 1 to 6\n");
		printf(	"1.Display files in home directory\n"
				"2.Download\n"
				"3.Upload\n"
				"4.Rename\n"
				"5.Remove\n"
				"6.Quit\n");
		scanf("%d", &option);
		if (isdigit(option) && (option < 7 && option > 0))
		{
			switch (option)
			{
			case 1:
				Display();
				break;
			case 2:
				Download();
				break;
			case 3:
				Upload();
				break;
			case 4:
				Rename();
				break;
			case 5:
				Remove();
				break;
			default:
				break;
			}
		}
	} while (option != 6);

	CloseConnection(controlConnectSocket);
}