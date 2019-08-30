#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define PORT 8087
char* SERVER_IP;
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#pragma comment(lib, "WS2_32")

BOOL  GetRealIpByDomainName(char* szHost, char szIp[50][100], int* nCount)
{
	WSADATA wsaData;
	HOSTENT* pHostEnt;
	int nAdapter = 0;
	struct sockaddr_in   sAddr;
	if (WSAStartup(0x0101, &wsaData))
	{
		printf(" gethostbyname error for host:\n");
		return FALSE;
	}

	pHostEnt = gethostbyname(szHost);
	if (pHostEnt)
	{
		while (pHostEnt->h_addr_list[nAdapter])
		{
			memcpy(&sAddr.sin_addr.s_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);

			sprintf_s(szIp[nAdapter], "%s", inet_ntoa(sAddr.sin_addr));

			nAdapter++;
		}

		*nCount = nAdapter;
	}
	else
	{
		DWORD  dwError = GetLastError();
		*nCount = 0;
	}
	WSACleanup();
	return TRUE;
}



int main(int argc, char* argv[])
{
	system("title ��� 2nd ver2.31");
	// ���ص�������Ӧʵ��IP�ĸ���
	int nIpCount = 0;
	// ���ص�������Ӧʵ��I�б�
	char szIpList[50][100];
	// ����
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "mccolor.tpddns.cn");
	GetRealIpByDomainName(szDomain, szIpList, &nIpCount);

	printf("����������IP=%s\n", szIpList[0]);

	SERVER_IP = szIpList[0];

	while (true) {
	// ��ʼ��socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 0);
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		printf("Init socket dll error!");
		exit(1);
	}

	//����socket
	SOCKET c_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == c_Socket)
	{
		printf("Create Socket Error!");
		system("pause");
		exit(1);
	}

	//ָ������˵ĵ�ַ
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	server_addr.sin_port = htons(PORT);

	if (SOCKET_ERROR == connect(c_Socket, (LPSOCKADDR)& server_addr, sizeof(server_addr)))
	{
		printf("Can Not Connect To Server IP!\n");
		system("pause");
		exit(1);
	}

	//�����ļ���
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);

	if (argc < 2) {
		printf("�������ļ���\n");
		scanf("%s", file_name);
	}
	else strcpy(file_name, argv[1]);

	printf("FileName=%s", file_name);

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));

	//������������ļ���
	if (send(c_Socket, buffer, BUFFER_SIZE, 0) < 0)
	{
		printf("Send File Name Failed\n");
		system("pause");
		exit(1);
	}

	//���ļ���׼��д��
	FILE* fp = fopen(file_name, "wb"); //windows����"wb",��ʾ��һ��ֻд�Ķ������ļ�
	if (NULL == fp)
	{
		printf("File: %s Can Not Open To Write\n", file_name);
		system("pause");
		exit(1);
	}
	else
	{
		memset(buffer, 0, BUFFER_SIZE);
		int length = 0;
		while ((length = recv(c_Socket, buffer, BUFFER_SIZE, 0)) > 0)
		{
			if (fwrite(buffer, sizeof(char), length, fp) < length)
			{
				printf("File: %s Write Failed\n", file_name);
				break;
			}
			memset(buffer, 0, BUFFER_SIZE);
		}

		printf("Receive File: %s From Server Successful!\n", file_name);

	}

	fclose(fp);
	closesocket(c_Socket);
	WSACleanup();
	char command[1024];
	sprintf(command, "ren %s %%date:~0,4%%%%date:~5,2%%%%date:~8,2%%%%time:~0,2%%%%time:~3,2%%%%time:~6,2%%.jpg", file_name);
	system(command);

	//�ͷ�winsock��
	Sleep(10000);
}

	return 0;
}