// dllmain.cpp : Defines the entry point for the DLL application.

#include "pch.h"

#pragma data_seg(".shared")
DWORD g_InjectorId = 0;
#pragma data_seg()
#pragma comment(linker, "/section:.shared,RWS")

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_PROCESS_DETACH:
			PostThreadMessage(g_InjectorId, WM_QUIT, 0, 0);
			break;
	}
	return TRUE;
}

LRESULT CALLBACK HookFunc(int code, WPARAM wParam, LPARAM lParam) {
	if (code == HC_ACTION) {
		auto msg = (MSG*)lParam;
		if (msg->message == WM_CHAR) {
			if (msg->wParam == 'H')
				msg->wParam = 0;
			PostThreadMessage(g_InjectorId, WM_APP, msg->wParam, msg->lParam);
		}
	}
	return CallNextHookEx(nullptr, code, wParam, lParam);
}

void WINAPI SetNotifyTid(DWORD tid) {
	g_InjectorId = tid;
}
