// Close.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: close <pid>\n");
		return 0;
	}

	int pid = atoi(argv[1]);

	EnumWindows([](HWND hWnd, auto p) {
		DWORD id;
		DWORD targetPid = (DWORD)p;
		GetWindowThreadProcessId(hWnd, &id);
		if (id == targetPid) {
			printf("Posting WM_CLOSE...\n");
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return FALSE;
		}
		return TRUE;
		}, pid);

	return 0;
}

