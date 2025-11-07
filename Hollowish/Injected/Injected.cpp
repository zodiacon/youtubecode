#include <Windows.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPTSTR cmdLine, int show) {
	WCHAR buffer[128];
	wsprintfW(buffer, L"Injected into PID: %u", GetCurrentProcessId());
	MessageBox(nullptr, buffer, L"Injected", MB_OK | MB_ICONINFORMATION);

	return 0;
}