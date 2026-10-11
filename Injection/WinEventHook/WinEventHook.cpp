// WinEventHook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

void CALLBACK OnEvent(
    HWINEVENTHOOK hWinEventHook,
    DWORD         event,
    HWND          hwnd,
    LONG          idObject,
    LONG          idChild,
    DWORD         idEventThread,
    DWORD         dwmsEventTime) {
    printf("Event %u hwnd: 0x%p Object: %d child: %d TID: %d\n",
        event, hwnd, idObject, idChild, idEventThread);
}

int main() {
    WCHAR path[MAX_PATH];
    GetModuleFileName(nullptr, path, _countof(path));
    *wcsrchr(path, L'\\') = 0;
    wcscat_s(path, L"\\SimpleDll.dll");
    auto hDll = LoadLibrary(path);
    auto onEvent = (decltype(OnEvent)*)GetProcAddress(hDll, "OnEvent");
	auto hook = SetWinEventHook(EVENT_MIN, EVENT_MAX, hDll, onEvent,
		67280, 0, WINEVENT_INCONTEXT | WINEVENT_SKIPOWNPROCESS);

    Sleep(INFINITE);

    //MSG msg;
    //while (GetMessage(&msg, nullptr, 0, 0))
    //    DispatchMessageW(&msg);

	return 0;

}
