// SimpleCdb.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Debugger.h"
#include <stdio.h>

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

int main() {
	CoInitialize(nullptr);

	static Debugger dbg;
	dbg.Init();

	WCHAR name[] = L"notepad";
	auto pid = LaunchExe(name);
	if (!pid)
		return 1;

	Sleep(500);
	dbg.AttachToProcess(pid);

	char command[256];
	bool exit = false;

	SetConsoleCtrlHandler([](auto) {
		dbg.Break();
		return TRUE;
		}, TRUE);

	while (!exit) {
		auto state = dbg.GetState();
		switch (state) {
			case DEBUG_STATUS_NO_DEBUGGEE:
				exit = true;
				continue;

			case DEBUG_STATUS_GO:
			case DEBUG_STATUS_STEP_BRANCH:
			case DEBUG_STATUS_STEP_INTO:
			case DEBUG_STATUS_STEP_OVER:
				dbg.WaitForEvent();
				if (dbg.GetState() == DEBUG_STATUS_BREAK)
					dbg.DisplayState();
				break;
		}
		dbg.DisplayPrompt();
		printf(" ");
		gets_s(command, sizeof(command));
		dbg.ExecuteCommand(command);
	}

	return 0;
}
