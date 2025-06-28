// Sample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

struct LogMessage {
	DWORD Pid;
	DWORD Tid;
	char Title[20];
	char Log[200];
};

int main() {
	const UINT MyMsg = WM_USER + 100;
	HWND hWnd = FindWindow(nullptr, L"Logging App");
	if (!hWnd) {
		printf("Logger not found!\n");
		return 1;
	}

	for (;;) {
		printf("Title: ");
		char title[20];
		gets_s(title);
		if (_stricmp(title, "q") == 0)
			break;

		printf("Message: ");
		char log[200];
		gets_s(log);

		LogMessage msg;
		msg.Tid = GetCurrentThreadId();
		msg.Pid = GetCurrentProcessId();
		strcpy_s(msg.Title, title);
		strcpy_s(msg.Log, log);

		// send the log
		COPYDATASTRUCT cds;
		cds.dwData = 0;
		cds.lpData = &msg;
		cds.cbData = sizeof(msg);
		SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cds);
	}

	return 0;
}

