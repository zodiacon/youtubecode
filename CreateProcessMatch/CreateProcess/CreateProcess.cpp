// CreateProcess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

DWORD CreateProcessAsUserDemo(PCWSTR username, PCWSTR password, PWSTR exe) {
	HANDLE hToken;
	if (!LogonUser(username, L".", password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, &hToken))
		return 0;

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcessAsUser(hToken, nullptr, exe, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return pi.dwProcessId;
}

DWORD CreateProcessWithTokenDemo(PCWSTR username, PCWSTR password, PWSTR exe) {
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	if (CreateProcessWithLogonW(username, L".", password, 0, nullptr, exe, 0,
		nullptr, nullptr, &si, &pi)) {
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	return pi.dwProcessId;
}

int main() {
	WCHAR name[] = L"cmd.exe";
	DWORD pid = CreateProcessWithTokenDemo(L"alice", L"123456", name);
	if (pid)
		printf("PID: %u\n", pid);

	return 0;
}
