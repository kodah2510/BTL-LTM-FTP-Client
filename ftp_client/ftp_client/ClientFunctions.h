#include<stdio.h>
#include<conio.h>
#include<winsock.h>

bool Login(SOCKET* controlConnectSocket);
int Display(SOCKET* controlConnectSocket,SOCKET* dataSocket);
void Download();
void Upload();
void Rename();
void Remove();