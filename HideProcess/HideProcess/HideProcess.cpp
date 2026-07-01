// HideProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: hideprocess <pid>\n");
		return 0;
	}

	HANDLE hDevice = CreateFile(L"\\\\.\\KHideProcess", GENERIC_WRITE,
		0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hDevice == INVALID_HANDLE_VALUE) {
		printf("Error: %u\n", GetLastError());
		return 1;
	}

	auto pid = strtoul(argv[1], nullptr, 0);
	if (DWORD ret; WriteFile(hDevice, &pid, sizeof(pid), &ret, nullptr)) {
		printf("Success!\n");
	}
	else {
		printf("Error: %u\n", GetLastError());
	}
	CloseHandle(hDevice);
	return 0;
}
