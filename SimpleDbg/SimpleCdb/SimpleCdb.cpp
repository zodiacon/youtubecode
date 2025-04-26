// SimpleCdb.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Debugger.h"
#include <stdio.h>

DWORD LaunchExe(PWSTR path) {
	STARTUPINFO si{ sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcessW(nullptr, path, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return pi.dwProcessId;
	}
	return 0;
}

int main() {
	CoInitialize(nullptr);

	Debugger dbg;
	dbg.Init();

	WCHAR name[] = L"notepad";
	auto pid = LaunchExe(name);
	if (!pid)
		return 1;

	Sleep(2000);
	dbg.AttachToProcess(pid);

	return 0;
}
