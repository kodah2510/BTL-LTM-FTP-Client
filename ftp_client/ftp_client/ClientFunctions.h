#include<stdio.h>
#include<conio.h>
#include<winsock.h>

bool Login(SOCKET* controlConnectSocket);
void Display(SOCKET* controlConnectSocket);
void Download(SOCKET* controlConnectSocket);
void Upload(SOCKET* controlConnectSocket);
void Rename(SOCKET* controlConnectSocket);
void Remove(SOCKET* controlConnectSocket);