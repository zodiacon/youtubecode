// SimpleSvc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include "Logger.h"
#include <stdio.h>

WCHAR g_ServiceName[] = L"Simple";
SERVICE_STATUS_HANDLE g_Service;
SERVICE_STATUS g_Status;
HANDLE g_hLogger;
HANDLE g_hMailslot;

void SetStatus(DWORD state) {
	g_Status.dwCurrentState = state;
	SetServiceStatus(g_Service, &g_Status);
}

void WINAPI SimpleHandler(DWORD dwControl) {
	switch (dwControl) {
		case SERVICE_CONTROL_STOP:
			CloseHandle(g_hLogger);
			CloseHandle(g_hMailslot);
			SetStatus(SERVICE_STOPPED);
			break;
	}
}

const char* LogLevels[] = {
	"Critical",
	"Error",
	"Warning",
	"Info",
	"Debug",
};


void WINAPI SimpleMain(DWORD dwNumServicesArgs, LPWSTR* lpServiceArgVectors) {
	g_Service = RegisterServiceCtrlHandler(g_ServiceName, SimpleHandler);

	SetStatus(SERVICE_START_PENDING);

	// init

	SECURITY_DESCRIPTOR sd;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, nullptr, FALSE);
	
	SECURITY_ATTRIBUTES sa{ sizeof(sa), &sd };
	g_hMailslot = CreateMailslot(L"\\\\.\\mailslot\\Logger", 0, 0, &sa);

	g_hLogger = CreateFile(L"d:\\temp\\SvcLogger.log", FILE_APPEND_DATA, FILE_SHARE_READ, nullptr,
		OPEN_ALWAYS, 0, nullptr);

	if (g_hMailslot == INVALID_HANDLE_VALUE || g_hLogger == INVALID_HANDLE_VALUE) {
		SetStatus(SERVICE_STOPPED);
		return;
	}

	SetStatus(SERVICE_RUNNING);

	DWORD size, messages;
	BYTE buffer[1024];
	char output[1024];
	while (GetMailslotInfo(g_hMailslot, nullptr, &size, &messages, nullptr)) {
		if (size == 0 || messages == MAILSLOT_NO_MESSAGE)
			continue;

		DWORD read = 0;
		ReadFile(g_hMailslot, buffer, size, &read, nullptr);
		if (read) {
			DWORD written;
			SYSTEMTIME st;
			GetLocalTime(&st);
			auto msg = (LogMessage*)buffer;
			sprintf_s(output, "%02d:%02d:%02d.%03d %s %s\n",
				st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
				LogLevels[(int)msg->Level], msg->Text);

			WriteFile(g_hLogger, output, (DWORD)strlen(output), &written, nullptr);
		}
	}
}

int main() {
	SERVICE_TABLE_ENTRY entries[] = {
		{ g_ServiceName, SimpleMain },
		{ nullptr }
	};

	g_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	g_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

	if (!StartServiceCtrlDispatcher(entries))
		return -1;

	return 0;
}

