// CreateStreams.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main() {
	HANDLE hFile = CreateFile(L"d:\\Pavel\\hello.txt", GENERIC_READ, 0,
		nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) {
		auto size = GetFileSize(hFile, nullptr);
		printf("Size of file: %u\n", size);
	}

	hFile = CreateFile(L"d:\\Pavel\\hello.txt:mydata", GENERIC_WRITE, 0,
		nullptr, OPEN_ALWAYS, 0, nullptr);
	if (hFile != INVALID_HANDLE_VALUE) {
		auto size = GetFileSize(hFile, nullptr);
		printf("Size: %u\n", size);
		char text[] = "Hello, Stream!!!";
		DWORD ret;
		for (int i = 0; i < 10000; i++) {
			WriteFile(hFile, text, sizeof(text), &ret, nullptr);
		}
		CloseHandle(hFile);
	}
	return 0;
}
