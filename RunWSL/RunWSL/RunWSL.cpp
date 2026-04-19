// RunWSL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <wslapi.h>
#include <stdio.h>
#include <Psapi.h>

//#pragma comment(lib, "wslapi.lib")

int main() {
	HANDLE hProcess;
	auto pWslLaunch = (decltype(WslLaunch)*)GetProcAddress(LoadLibrary(L"wslapi.dll"), "WslLaunch");

	auto hr = pWslLaunch(L"Ubuntu", L"ps aux", FALSE,
		GetStdHandle(STD_INPUT_HANDLE), GetStdHandle(STD_OUTPUT_HANDLE), GetStdHandle(STD_ERROR_HANDLE),
		&hProcess);
	if (SUCCEEDED(hr)) {
		WCHAR path[MAX_PATH];
		if (GetProcessImageFileNameW(hProcess, path, _countof(path))) {
			printf("Path: %ws\n", path);
		}
		CloseHandle(hProcess);
	}
}
