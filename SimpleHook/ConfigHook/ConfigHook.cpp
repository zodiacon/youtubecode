// ConfigHook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

__declspec(dllimport) WCHAR HiddenProcessName[32];
__declspec(dllimport) DWORD HiddenPid;

void Usage() {
	printf("Usage: ConfigDll hide <pname | pid>\n");
}

int wmain(int argc, const wchar_t* argv[]) {
	if (argc < 2) {
		Usage();
		return 0;
	}

	if (_wcsicmp(argv[1], L"hide") != 0) {
		Usage();
		return 1;
	}

	if (argc == 2) {
		HiddenProcessName[0] = 0;
		HiddenPid = 0;
	}
	else {
		auto pid = _wtoi(argv[2]);
		if (pid == 0)
			wcscpy_s(HiddenProcessName, argv[2]);
		else
			HiddenPid = pid;
	}
	return 0;
}

