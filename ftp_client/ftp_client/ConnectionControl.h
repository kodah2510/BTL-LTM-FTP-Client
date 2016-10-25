#include<stdio.h>
#include<winsock.h>
//ham thiet lap ket noi bao gom tao wsa, 
//khoi tao socket control va ket noi den server
bool InitializeConnection(SOCKET* controlConnectSocket,SOCKET* dataSocket);
void CloseConnection(SOCKET controlConnectSocket);
