#include<stdio.h>
#include<conio.h>
#include<winsock.h>

bool Login(SOCKET* controlConnectSocket);
void Display(SOCKET* controlConnectSocket);
void Download();
void Upload();
void Rename(SOCKET* controlConnectSocket);
void Remove(SOCKET* controlConnectSocket);