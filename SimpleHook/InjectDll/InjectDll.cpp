// InjectDll.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	if (argc < 3) {
		printf("Usage: InjectDll <pid> <dllpath>\n");
		return 0;
	}

	auto pid = atoi(argv[1]);

	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION |
		PROCESS_CREATE_THREAD, FALSE, pid);
	if (!hProcess) {
		printf("Error opening process (%u)\n", GetLastError());
		return 1;
	}

	auto p = VirtualAllocEx(hProcess, nullptr, 1 << 12, MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE);

	WriteProcessMemory(hProcess, p, argv[2], strlen(argv[2]) + 1, nullptr);

	auto hThread = CreateRemoteThread(hProcess, nullptr, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(
			GetModuleHandle(L"kernel32.dll"), "LoadLibraryA"),
		p, 0, nullptr);
	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	CloseHandle(hProcess);
}

