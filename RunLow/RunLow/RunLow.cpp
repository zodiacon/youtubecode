// RunLow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <sddl.h>

int wmain(int argc, wchar_t* argv[]) {
	if (argc < 2) {
		printf("Usage: RunLow <exe>\n");
		return 0;
	}

	HANDLE hToken;
	if (!OpenProcessToken(GetCurrentProcess(), MAXIMUM_ALLOWED, &hToken))
		return 1;

	HANDLE hNewToken;
	if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, nullptr, SecurityImpersonation,
		TokenPrimary, &hNewToken))
		return 1;

	PSID sid;
	if (!ConvertStringSidToSid(L"S-1-16-4096", &sid))
		return 1;

	TOKEN_MANDATORY_LABEL label;
	label.Label.Attributes = 0;
	label.Label.Sid = sid;
	if(!SetTokenInformation(hNewToken, TokenIntegrityLevel, &label, sizeof(label)))
		return 1;

	LocalFree(sid);
	PROCESS_INFORMATION pi;
	STARTUPINFO si{ sizeof(si) };
	if (CreateProcessAsUser(hNewToken, nullptr, argv[1], nullptr, nullptr, FALSE,
		0, nullptr, nullptr, &si, &pi)) {
		printf("Success!\n");
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else {
		printf("Error! %u\n", GetLastError());
	}
	CloseHandle(hToken);
	CloseHandle(hNewToken);

}
