// App.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include "..\SimpleSvc\Logger.h"

int main() {
	HANDLE hLogger = CreateFile(L"\\\\.\\Mailslot\\Logger", GENERIC_WRITE, 0, nullptr,
		OPEN_EXISTING, 0, nullptr);

	if (hLogger == INVALID_HANDLE_VALUE) {
		printf("Error opening mailslot (%u)\n", GetLastError());
		return 1;
	}

	struct {
		LogMessage msg;
		char buffer[500];
	} msg;
	DWORD written;
	for (int i = 0; i < 10; i++) {
		Sleep(GetTickCount64() % 700);

		msg.msg.Level = (LogLevel)(GetTickCount64() % (int)LogLevel::_Count);
		auto n = sprintf_s(msg.msg.Text, sizeof(msg.buffer), "This is a message from PID:TID %u:%u",
			GetCurrentProcessId(), GetCurrentThreadId());

		WriteFile(hLogger, &msg, n + sizeof(LogMessage), &written, nullptr);
	}
}

