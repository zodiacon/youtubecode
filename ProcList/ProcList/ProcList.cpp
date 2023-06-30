// ProcList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <WtsApi32.h>
#include <sddl.h>
#include <string>
#include <Psapi.h>

#pragma comment(lib, "wtsapi32")

void EnumProcToolhelp() {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	Process32First(hSnapshot, &pe);
	do {
		printf("PID: %6u PPID: %6u T: %3u Name: %ws\n",
			pe.th32ProcessID, pe.th32ParentProcessID,
			pe.cntThreads, pe.szExeFile);
	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);
}

std::wstring SidToStringSid(PSID sid) {
	if (sid == nullptr)
		return L"";

	PWSTR ssid;
	if (ConvertSidToStringSid(sid, &ssid)) {
		std::wstring result(ssid);
		LocalFree(ssid);
		return result;
	}
	return L"";
}

void EnumProcWTS() {
	DWORD level = 1;
	WTS_PROCESS_INFO_EX* info;
	DWORD count;
	if (!WTSEnumerateProcessesEx(nullptr, &level, WTS_ANY_SESSION,
		(PWSTR*)&info, &count))
		return;

	for (DWORD i = 0; i < count; i++) {
		auto& p = info[i];
		printf("PID: %6u Session: %u T: %3u H: %6u Name: %ws Mem: %u MB SID: %ws\n",
			p.ProcessId, p.SessionId, p.NumberOfThreads,
			p.HandleCount, p.pProcessName, p.WorkingSetSize >> 20,
			SidToStringSid(p.pUserSid).c_str());
	}

	WTSFreeMemory(info);

}

void EnumProcessesRaw() {
	DWORD pids[1024];
	DWORD needed;
	if (!EnumProcesses(pids, sizeof(pids), &needed))
		return;

	for (DWORD i = 0; i < needed / sizeof(DWORD); i++) {
		printf("PID: %6u", pids[i]);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pids[i]);
		if (hProcess) {
			WCHAR path[MAX_PATH];
			DWORD size = _countof(path);
			if (QueryFullProcessImageName(hProcess, 0, path, &size)) {
				printf(" Name: %ws", path);
			}
			CloseHandle(hProcess);
		}
		printf("\n");
	}
}

int main() {
	EnumProcessesRaw();
}
