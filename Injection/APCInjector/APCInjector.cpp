// APCInjector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <TlHelp32.h>

std::vector<DWORD> EnumThreads(DWORD pid) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return {};

	THREADENTRY32 te;
	te.dwSize = sizeof(te);

	Thread32First(hSnapshot, &te);
	std::vector<DWORD> tids;

	while (Thread32Next(hSnapshot, &te)) {
		if (te.th32OwnerProcessID == pid)
			tids.push_back(te.th32ThreadID);
	}
	CloseHandle(hSnapshot);
	return tids;
}

int main(int argc, const char* argv[]) {
	if (argc < 3) {
		printf("Usage: Injector <pid> <dllpath>\n");
		return 0;
	}

	auto pid = atoi(argv[1]);

	auto tids = EnumThreads(pid);
	if (tids.empty()) {
		printf("No threads!\n");
		return 1;
	}

	HANDLE hProcess = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION |
		PROCESS_CREATE_THREAD, FALSE, pid);
	if (!hProcess) {
		printf("Error opening process (%u)\n", GetLastError());
		return 1;
	}

	auto p = VirtualAllocEx(hProcess, nullptr, 1 << 12, MEM_COMMIT | MEM_RESERVE,
		PAGE_READWRITE);

	WriteProcessMemory(hProcess, p, argv[2], strlen(argv[2]) + 1, nullptr);

	for (auto tid : tids) {
		HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, tid);
		if (hThread) {
			QueueUserAPC((PAPCFUNC)GetProcAddress(GetModuleHandle(L"kernel32"),
				"LoadLibraryA"), hThread, (ULONG_PTR)p);
			CloseHandle(hThread);
		}
	}

	CloseHandle(hProcess);

	return 0;
}
