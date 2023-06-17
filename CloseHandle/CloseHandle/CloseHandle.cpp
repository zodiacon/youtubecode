// CloseHandle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>


int main(int argc, const char* argv[]) {
	if (argc < 3) {
		printf("Usage: CloseHandle <pid> <handle>\n");
		return 0;
	}

	int pid = strtol(argv[1], nullptr, 0);
	HANDLE handle = (HANDLE)(ULONG_PTR)strtol(argv[2], nullptr, 0);

	HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, pid);
	if (!hProcess) {
		printf("Error opening process (%u)\n", GetLastError());
		return 1;
	}

	if (DuplicateHandle(hProcess, handle, nullptr, nullptr,
		0, FALSE, DUPLICATE_CLOSE_SOURCE)) {
		printf("Success!\n");
	}
	else {
		printf("Error duplicating handle (%u)\n", GetLastError());
	}
	CloseHandle(hProcess);
	return 0;
}
