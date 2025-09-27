// Sessions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <WtsApi32.h>

#pragma comment(lib, "wtsapi32")

void DisplayDesktops(PCWSTR name) {
	auto hWinSta = OpenWindowStation(name, FALSE, WINSTA_ENUMDESKTOPS);
	if (hWinSta) {
		EnumDesktops(hWinSta, [](auto deskName, auto) {
			printf("  Desktop: %ws\n", deskName);
			//EnumDesktopWindows(
			return TRUE;
			}, 0);

		CloseWindowStation(hWinSta);
	}
}

int main() {
	DWORD level = 1;
	PWTS_SESSION_INFO_1 info;
	DWORD count;
	if (WTSEnumerateSessionsEx(nullptr, &level, 0, &info, &count)) {
		for (DWORD i = 0; i < count; i++) {
			printf("Session %u: %ws User: %ws\\%ws (%u)\n",
				info[i].SessionId, info[i].pSessionName,
				info[i].pDomainName, info[i].pUserName, info[i].State);
		}
		WTSFreeMemory(info);
	}
	printf("Windows stations\n");
	EnumWindowStations([](auto name, auto) {
		printf(" %ws\n", name);
		DisplayDesktops(name);
		return TRUE;
		}, 0);

	return 0;
}
