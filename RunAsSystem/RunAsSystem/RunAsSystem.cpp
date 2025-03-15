// RunAsSystem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>

DWORD FindProcess(PCWSTR exeName) {
	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	Process32First(hSnapshot, &pe);

	DWORD pid = 0;
	while (Process32Next(hSnapshot, &pe)) {
		if (_wcsicmp(exeName, pe.szExeFile) == 0) {
			pid = pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return pid;
}

int main() {
	auto pid = FindProcess(L"smss.exe");
	if (pid == 0)
		return 1;

	auto hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (!hProcess)
		return 1;

	HANDLE hToken;
	if (!OpenProcessToken(hProcess, TOKEN_DUPLICATE, &hToken))
		return 1;

	HANDLE hSysToken;
	if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, nullptr, SecurityIdentification,
		TokenPrimary, &hSysToken))
		return 1;

	WCHAR name[] = L"notepad";
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };

	auto ok = CreateProcessWithTokenW(hSysToken, 0, nullptr, name,
		0, nullptr, nullptr, &si, &pi);
	if (ok)
		printf("Success!\n");
	else
		printf("Failed (%u)\n", GetLastError());

	return 0;

}
