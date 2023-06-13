// Kill.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: kill <pid>\n");
		return 0;
	}

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE,
		atoi(argv[1]));
	if (!hProcess) {
		printf("Error opening process (%u)\n", GetLastError());
		return 1;
	}

	if (TerminateProcess(hProcess, 1))
		printf("Success!\n");
	else
		printf("Error (%u)\n", GetLastError());
	
	CloseHandle(hProcess);
	return 0;
}

