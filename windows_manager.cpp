// windows_manager.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <conio.h>
#include "locale.h"
#include "windows.h"
#include <vector>

#define LINE_MAX		256
#define TITLE_MAX		1023

//EnumWindows(__in WNDENUMPROC lpEnumFunc, __in LPARAM lParam)
void quit(void);
int call(const char* line);

int _tmain(int argc, _TCHAR* argv[])
{
	printf("print 'help' to help you.\n");
	char line[LINE_MAX] = {0};
	while (printf("\\> ")) {
		gets_s(line, LINE_MAX);
		if (ferror(stdin)) {
			perror("ERROR: ");
			quit();
		}

		if (!line[0])
			continue;

#ifdef _DEBUG
//		printf("[COMMAND] %s\n", line);
#endif
		call(line);
	}

	quit();
	return 0;
}

void quit(void)
{
//	printf("print any key to exit..");
	while (!_kbhit());
	exit(0);
}

BOOL CALLBACK EnumFuncList(HWND hWnd, LPARAM lParam)
{
	std::vector<HWND> *windows = (std::vector<HWND>*)lParam;
	if (IsWindowVisible(hWnd))
		windows->push_back(hWnd);
	return TRUE;
}
std::vector<HWND> windows;
int call(const char* line)
{
	if (strcmp(line, "list") == 0)	{
		windows.clear();
		BOOL bRet = EnumDesktopWindows(NULL, EnumFuncList, (LPARAM)&windows);
		if (!bRet)
			printf("EnumDesktopWindows Failed.\n");

		setlocale(LC_ALL, ".936");
		std::vector<HWND>::iterator i = windows.begin();
		while (i != windows.end()) {
			int len = GetWindowTextLength(*i);
			if (len > 0)
			{
				TCHAR *title = new TCHAR[TITLE_MAX + 1];
				ZeroMemory(title, TITLE_MAX + 1);
				GetWindowText(*i, title, TITLE_MAX);
				_tprintf_l(_T("%d\t%s\n"), _get_current_locale(), i - windows.begin(), title);
			}
			i++;
		}
	}
	if (strncmp(line, "toggle", sizeof("toggle")) == 0)
	{
		char buf[10] = {0};
		printf("enter the window index you want to toggle visible: ");
		gets_s(buf, 10);
		unsigned idx = atoi(buf);
		if (idx >= 0 && idx < windows.size())
		{
			ShowWindow(windows[idx], IsWindowVisible(windows[idx]) ? SW_HIDE : SW_SHOW);
		}
	}

	if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0)
	{
		quit();
	}

	if (strcmp(line, "help") == 0)
	{
		printf("list\t\t\tlist the windows has title\n");
		printf("toggle\t\t\thide or show the a window\n");
		printf("quit\t\t\texit the program\n");
		printf("exit\t\t\texit the program\n");
	}
	return 0;
}

