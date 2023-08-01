// MemResCom.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

long long xsize = 102400, ysize = 102400;
int cell_size = 1024;

int FixMemory(void* p, DWORD code) {
	if (code != EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_SEARCH;

	VirtualAlloc(p, cell_size, MEM_COMMIT, PAGE_READWRITE);
	return EXCEPTION_CONTINUE_EXECUTION;
}

int main() {
	auto buffer = (char*)VirtualAlloc(nullptr, xsize * ysize * cell_size,
		MEM_RESERVE, PAGE_READWRITE);

	//VirtualAlloc(buffer, cell_size, MEM_COMMIT, PAGE_READWRITE);

	char text[] = "Hello, memory!";

	__try {
		strcpy_s(buffer, strlen(text) + 1, text);
	}
	__except (FixMemory(buffer, GetExceptionCode())) {
	}

	int x = 10, y = 100;
	auto p = buffer + (x + y * xsize) * cell_size;
	return 0;
}
