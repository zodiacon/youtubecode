// Simple.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "Simple.h"
#include <string>

extern "C" SIMPLE_API void __stdcall KillProcess(HWND hWnd, HINSTANCE hInstDll, LPSTR cmdLine, int show) {
	int pid = atoi(cmdLine);
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (hProcess) {
		TerminateProcess(hProcess, 1);
		CloseHandle(hProcess);
	}
}
