#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<cstdio>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	system("title ��� 2nd ver2.31");
	//��ʼ��socket��Դ 
	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0)
		return 0;

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//����������socket
	if (INVALID_SOCKET == serSocket)
	{
		printf("SocketError!\n");
		return 0;
	}

	//���ô���Э�顢�˿��Լ�Ŀ�ĵ�ַ 
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(serSocket, (sockaddr*)& serAddr, sizeof(serAddr)) == SOCKET_ERROR)	 //��socket�󶨵�ַ 
	{
		printf("BindError!\n");
		closesocket(serSocket);
		return 0;
	}

	sockaddr_in clientAddr;
	int iAddrlen = sizeof(clientAddr);
	char buff[1024];	//�������ջ����ֽ����� 
	while (true)
	{
		memset(buff, 0, 1024);	//��ս��ջ�������
		//��ʼ�������� 
		int len = recvfrom(serSocket, buff, 1024, 0, (sockaddr*)& clientAddr, &iAddrlen);
		if (len > 0)
		{
			printf("[IP=%s] %s\n", inet_ntoa(clientAddr.sin_addr),buff);

			//	sendto(serSocket,buff,1024,0,(sockaddr*)&clientAddr,iAddrlen);
		}
	}

	closesocket(serSocket);		//�ر�socket 
	WSACleanup();

	return 0;
}