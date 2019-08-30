#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<cstdio>
using namespace std;
#pragma comment(lib,"ws2_32.lib")

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
	system("title 监控 2nd ver2.31");
	char* IP;

	// 返回的域名对应实际IP的个数
	int nIpCount = 0;
	// 返回的域名对应实际I列表
	char szIpList[50][100];
	// 域名
	char szDomain[256] = { 0 };
	char szIp[1024] = { 0 };
	strcpy_s(szDomain, "mccolor.tpddns.cn");
	GetRealIpByDomainName(szDomain, szIpList, &nIpCount);

	printf("公共服务器IP=%s\n", szIpList[0]);
	IP = szIpList[0];


	WSADATA WSAData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &WSAData) != 0)
		return 0;

	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == clientSocket)
	{
		printf("SocketError!\n");
		system("pause");
		return 0;
	}

	sockaddr_in dstAddr;
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_port = htons(8888);
	dstAddr.sin_addr.S_un.S_addr = inet_addr(IP);
	char sendData[1024];
	strcpy(sendData, argv[1]);
	sendto(clientSocket, sendData, strlen(sendData), 0, (sockaddr*)& dstAddr, sizeof(dstAddr));

	closesocket(clientSocket);
	WSACleanup();

	return 0;
}