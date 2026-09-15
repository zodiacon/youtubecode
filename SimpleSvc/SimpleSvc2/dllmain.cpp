// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include <format>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

SERVICE_STATUS_HANDLE g_hService;
HANDLE g_hShutdown;

void SetStatus(DWORD state) {
	SERVICE_STATUS status{};
	status.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
	status.dwCurrentState = state;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	SetServiceStatus(g_hService, &status);
}

void Handler(DWORD control) {
	switch (control) {
	case SERVICE_CONTROL_STOP:
		SetEvent(g_hShutdown);
		break;
	}
}

DWORD __stdcall Worker(PVOID) {
	SetStatus(SERVICE_RUNNING);

	auto tick = GetTickCount64();
	while (WaitForSingleObject(g_hShutdown, 1000) == WAIT_TIMEOUT) {
		OutputDebugStringA(std::format("SimpleSvc2 elapsed: {:5}\n", GetTickCount64() - tick).c_str());
	}
	CloseHandle(g_hShutdown);
	SetStatus(SERVICE_STOPPED);

	return 0;
}

extern "C" void __declspec(dllexport) ServiceMain(int argc, PWSTR* argv) {
	g_hService = RegisterServiceCtrlHandler(L"SimpleSvc2", Handler);
	g_hShutdown = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	SetStatus(SERVICE_START_PENDING);

	CloseHandle(CreateThread(nullptr, 0, Worker, nullptr, 0, nullptr));
}
