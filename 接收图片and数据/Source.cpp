#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<cstdio>
#include<Windows.h>
#include <thread>


int main() {
	system("title 监控 2nd ver2.31");
	system("start frp.bat");
	system("start .\\接收端.exe");
	system("start .\\FileClient.exe screen.jpg");
	printf("全部开启完毕\n");

	system("pause");
}