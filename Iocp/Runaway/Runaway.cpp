// Runaway.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>

int main() {
	WCHAR name[] = L"Notepad";
	STARTUPINFO si{ sizeof(si) };
	si.wShowWindow = SW_SHOWMINNOACTIVE;
	si.dwFlags = STARTF_USESHOWWINDOW;
	PROCESS_INFORMATION pi;

	for (;;) {
		if (CreateProcess(nullptr, name, nullptr, nullptr, FALSE, 0,
			nullptr, nullptr, &si, &pi)) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		Sleep(1000);
	}
	return 0;
}

