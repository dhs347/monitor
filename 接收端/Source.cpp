#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<cstdio>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	system("title 监控 2nd ver2.31");
	//初始化socket资源 
	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0)
		return 0;

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//创建服务器socket
	if (INVALID_SOCKET == serSocket)
	{
		printf("SocketError!\n");
		return 0;
	}

	//设置传输协议、端口以及目的地址 
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(serSocket, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR)	 //将socket绑定地址 
	{
		printf("BindError!\n");
		closesocket(serSocket);
		return 0;
	}

	sockaddr_in clientAddr;
	int iAddrlen = sizeof(clientAddr);
	char buff[1024];	//建立接收缓存字节数组 
	while (true)
	{
		memset(buff, 0, 1024);	//清空接收缓存数组
		//开始接收数据 
		int len = recvfrom(serSocket, buff, 1024, 0, (sockaddr*)& clientAddr, &iAddrlen);
		if (len > 0)
		{
			printf("[IP=%s] %s\n", inet_ntoa(clientAddr.sin_addr),buff);

			//	sendto(serSocket,buff,1024,0,(sockaddr*)&clientAddr,iAddrlen);
		}
	}

	closesocket(serSocket);		//关闭socket 
	WSACleanup();

	return 0;
}