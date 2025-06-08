// ProcNotify.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include "../KProcNotify/ProcNotifyShared.h"

void DisplayTime(ULONG64 time) {
	FILETIME ft;
	FileTimeToLocalFileTime((FILETIME*)&time, &ft);
	SYSTEMTIME st;
	FileTimeToSystemTime(&ft, &st);
	printf("%02d:%02d:%02d.%03d",
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

void DisplayBuffer(BYTE* data, DWORD size) {
	while (size) {
		auto header = (EventHeader*)data;
		DisplayTime(header->Time);
		switch (header->Type) {
			case EventType::ProcessCreate:
			{
				auto evt = (ProcessCreate*)header;
				printf(" Process Create PID: %u PPID: %u Command Line: %ws\n",
					evt->ProcessId, evt->ParentId, evt->CommandLine);
				break;
			}
			case EventType::ProcessExit:
				auto evt = (ProcessExit*)header;
				printf(" Process Exit PID: %u Exit Code: %d\n",
					evt->ProcessId, evt->ExitCode);
				break;

		}
		data += header->Size;
		size -= header->Size;
	}
}

int main() {
	HANDLE hDevice = CreateFile(L"\\\\.\\ProcNotify", GENERIC_READ,
		0, nullptr,
		OPEN_EXISTING, 0, nullptr);

	if (hDevice == INVALID_HANDLE_VALUE) {
		printf("Error: %u\n", GetLastError());
		return -1;
	}
	printf("Open handle: 0x%p\n", hDevice);

	BYTE buffer[4 << 10];
	DWORD read;
	while (ReadFile(hDevice, buffer, sizeof(buffer), &read, nullptr)) {
		if (read)
			DisplayBuffer(buffer, read);
		Sleep(500);
	}
	printf("ReadFile failed: %u\n", GetLastError());
	CloseHandle(hDevice);
	return 0;
}
