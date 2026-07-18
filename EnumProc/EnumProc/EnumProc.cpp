// EnumProc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>

bool EnablePrivilege(PCWSTR name) {
	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
		return false;

	LUID luid;
	bool result = false;
	if (LookupPrivilegeValue(nullptr, name, &luid)) {
		TOKEN_PRIVILEGES tp;
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), nullptr, nullptr)) {
			result = GetLastError() == ERROR_SUCCESS;
		}
	}
	CloseHandle(hToken);
	return result;
}

int main() {
	if (!EnablePrivilege(SE_DEBUG_NAME))
		printf("Debug privilege not available\n");

	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe{ sizeof(pe) };

	Process32First(hSnapshot, &pe);
	while(Process32Next(hSnapshot, &pe)) {
		printf("PID: %6u PPID: %6u Threads: %3u Name: %ws Path: ",
			pe.th32ProcessID, pe.th32ParentProcessID, pe.cntThreads, pe.szExeFile);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID);
		if (hProcess) {
			WCHAR path[MAX_PATH];
			DWORD size = _countof(path);
			if (QueryFullProcessImageName(hProcess, 0, path, &size)) {
				printf("%ws", path);
			}
			else {
				printf("<error in Query path: %u>", GetLastError());
			}
			CloseHandle(hProcess);
		}
		else {
			printf("<error in OpenProcess: %u>", GetLastError());
		}
		printf("\n");
	}

	CloseHandle(hSnapshot);
	return 0;
}

