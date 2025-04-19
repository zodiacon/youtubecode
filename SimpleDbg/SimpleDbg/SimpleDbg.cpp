// SimpleDbg.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

HANDLE g_hProcess;

DWORD LaunchExe(PWSTR path) {
	STARTUPINFO si{ sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcessW(nullptr, path, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return pi.dwProcessId;
	}
	return 0;
}

void DisplayImageName(PVOID p, bool unicode) {
	printf("Image name: ");
	PVOID address;
	if (ReadProcessMemory(g_hProcess, p, &address, sizeof(address), nullptr)) {
		BYTE path[1024]{};
		if (address && ReadProcessMemory(g_hProcess, address, path, sizeof(path) - 2, nullptr)) {
			printf(unicode ? "%ws" : "%s", (PVOID)path);
		}
	}
}

void DisplayFilePath(HANDLE hFile) {
	union {
		FILE_NAME_INFO path;
		BYTE _dummy[MAX_PATH * 2];
	} info;
	if (GetFileInformationByHandleEx(hFile, FileNameInfo, &info, sizeof(info)))
		printf("Path: %ws", info.path.FileName);
}

void DisplayCreateProcessInfo(CREATE_PROCESS_DEBUG_INFO const& info) {
	g_hProcess = info.hProcess;
	if (info.lpImageName)
		DisplayImageName(info.lpImageName, info.fUnicode);
	else
		DisplayFilePath(info.hFile);
	printf(" Main thread start: 0x%p", info.lpStartAddress);
}

void DisplayLoadDllInfo(LOAD_DLL_DEBUG_INFO const& info) {
	printf("Address: 0x%p ", info.lpBaseOfDll);
	if (info.lpImageName)
		DisplayImageName(info.lpImageName, info.fUnicode);
	else
		DisplayFilePath(info.hFile);
}

void DisplayCreateThreadInfo(CREATE_THREAD_DEBUG_INFO const& info) {
	printf("Start address: 0x%p", info.lpStartAddress);
}

int main(int argc, const char* argv[]) {
	//auto pid = strtoul(argv[1], nullptr, 0);

	WCHAR name[] = L"notepad";
	auto pid = LaunchExe(name);

	if (!DebugActiveProcess(pid))
		return 1;

	DEBUG_EVENT evt;
	bool exit = false;
	while (!exit && WaitForDebugEventEx(&evt, INFINITE)) {
		printf("(%d) PID: %u TID: %6u ",
			evt.dwDebugEventCode, evt.dwProcessId, evt.dwThreadId);

		switch (evt.dwDebugEventCode) {
			case CREATE_PROCESS_DEBUG_EVENT:
				DisplayCreateProcessInfo(evt.u.CreateProcessInfo);
				break;

			case EXIT_PROCESS_DEBUG_EVENT:
				printf("Exit code: %u", evt.u.ExitProcess.dwExitCode);
				exit = true;
				break;

			case LOAD_DLL_DEBUG_EVENT:
				DisplayLoadDllInfo(evt.u.LoadDll);
				break;

			case CREATE_THREAD_DEBUG_EVENT:
				DisplayCreateThreadInfo(evt.u.CreateThread);
				break;
		}
		printf("\n");
		ContinueDebugEvent(evt.dwProcessId, evt.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
	}

	return 0;
}
