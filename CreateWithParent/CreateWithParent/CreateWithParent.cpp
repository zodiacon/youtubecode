// CreateWithParent.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <memory>

int main(int argc, const char* argv[]) {
	int pid = argc > 1 ? atoi(argv[1]) : GetCurrentProcessId();

	HANDLE hParent = OpenProcess(PROCESS_CREATE_PROCESS, FALSE, pid);
	if (!hParent) {
		printf("Error opening process (%u)\n", GetLastError());
		return 1;
	}

	SIZE_T size;
	InitializeProcThreadAttributeList(nullptr, 1, 0, &size);
	auto buffer = std::make_unique<BYTE[]>(size);
	auto attributes = (PPROC_THREAD_ATTRIBUTE_LIST)buffer.get();
	InitializeProcThreadAttributeList(attributes, 1, 0, &size);

	UpdateProcThreadAttribute(attributes, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
		&hParent, sizeof(hParent), nullptr, nullptr);

	STARTUPINFOEX si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.lpAttributeList = attributes;

	WCHAR name[] = L"notepad";
	if (CreateProcess(nullptr, name, nullptr, nullptr, FALSE, EXTENDED_STARTUPINFO_PRESENT, 
		nullptr, nullptr, (STARTUPINFO*)&si, &pi)) {
		printf("Notepad created PID: %d\n", pi.dwProcessId);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
	else {
		printf("Failed to create process (%u)\n", GetLastError());
	}
	CloseHandle(hParent);
	DeleteProcThreadAttributeList(attributes);

	return 0;
}
