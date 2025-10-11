// WinVersion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <VersionHelpers.h>

#pragma comment(lib, "ntdll.lib")

extern "C" int RtlGetVersion(OSVERSIONINFOW* p);

int main() {
	OSVERSIONINFOW v{ sizeof(v) };
	RtlGetVersion(&v);

	if (IsWindows10OrGreater()) {
		printf("Windows 10!\n");
	}
	return 0;
}

