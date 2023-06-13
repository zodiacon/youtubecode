// Single.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main() {
	HANDLE h = CreateMutex(nullptr, FALSE, L"SingleInstanceDemo");
	if (!h) {
		printf("Error creating mutex (%u)\n", GetLastError());
		return 1;
	}

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		printf("Second instance... exiting\n");
		return 0;
	}

	printf("First Instance... press ENTER to exit...\n");
	char dummy[3];
	gets_s(dummy);
	CloseHandle(h);

	return 0;
}

