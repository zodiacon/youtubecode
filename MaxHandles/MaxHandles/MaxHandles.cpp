// MaxHandles.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int MaxHandles1() {
	int count = 0;
	for (;;) {
		HANDLE h = CreateJobObject(nullptr, nullptr);
		//HANDLE h = CreateMutex(nullptr, FALSE, nullptr);
		if (!h)
			break;
		count++;
	}
	return count;
}

int MaxHandles2() {
	HANDLE h = CreateMutex(nullptr, FALSE, nullptr);
	int count = 1;

	HANDLE hTarget;
	while (DuplicateHandle(GetCurrentProcess(), h,
		GetCurrentProcess(), &hTarget, 0, FALSE, DUPLICATE_SAME_ACCESS))
		count++;

	return count;
}

int main() {
	int count = MaxHandles1();
	printf("Total handles: %d\n", count);

	Sleep(INFINITE);
	return 0;
}
