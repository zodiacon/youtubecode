// ddd.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>


int wmain(int argc, const wchar_t* argv[]) {
	if (argc < 3) {
		printf("Usage: ddd <name> <path>\n");
		return 0;
	}

	if (DefineDosDevice(DDD_RAW_TARGET_PATH, argv[1], argv[2]))
		printf("Success!\n");
	else
		printf("Error (%u)\n", GetLastError());
	return 0;
}

