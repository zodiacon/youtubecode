// dllmain.cpp : Defines the entry point for the DLL application.
#include "framework.h"
#include <format>

#define DLL_PROCESS_VERIFIER 4

typedef struct _RTL_VERIFIER_THUNK_DESCRIPTOR {
	PCHAR ThunkName;
	PVOID ThunkOldAddress;
	PVOID ThunkNewAddress;
} RTL_VERIFIER_THUNK_DESCRIPTOR, * PRTL_VERIFIER_THUNK_DESCRIPTOR;

typedef struct _RTL_VERIFIER_DLL_DESCRIPTOR {
	PWCHAR DllName;
	ULONG DllFlags;
	PVOID DllAddress;
	PRTL_VERIFIER_THUNK_DESCRIPTOR DllThunks;
} RTL_VERIFIER_DLL_DESCRIPTOR, * PRTL_VERIFIER_DLL_DESCRIPTOR;

typedef void (NTAPI* RTL_VERIFIER_DLL_LOAD_CALLBACK) (
	PWSTR DllName,
	PVOID DllBase,
	SIZE_T DllSize,
	PVOID Reserved);
typedef void (NTAPI* RTL_VERIFIER_DLL_UNLOAD_CALLBACK) (
	PWSTR DllName,
	PVOID DllBase,
	SIZE_T DllSize,
	PVOID Reserved);
typedef void (NTAPI* RTL_VERIFIER_NTDLLHEAPFREE_CALLBACK) (
	PVOID AllocationBase,
	SIZE_T AllocationSize);

typedef struct _RTL_VERIFIER_PROVIDER_DESCRIPTOR {
	ULONG Length;
	PRTL_VERIFIER_DLL_DESCRIPTOR ProviderDlls;
	RTL_VERIFIER_DLL_LOAD_CALLBACK ProviderDllLoadCallback;
	RTL_VERIFIER_DLL_UNLOAD_CALLBACK ProviderDllUnloadCallback;

	PWSTR VerifierImage;
	ULONG VerifierFlags;
	ULONG VerifierDebug;

	PVOID RtlpGetStackTraceAddress;
	PVOID RtlpDebugPageHeapCreate;
	PVOID RtlpDebugPageHeapDestroy;

	RTL_VERIFIER_NTDLLHEAPFREE_CALLBACK ProviderNtdllHeapFreeCallback;
} RTL_VERIFIER_PROVIDER_DESCRIPTOR;

LPVOID WINAPI HookVirtualAlloc(
	_In_opt_ LPVOID lpAddress,
	_In_     SIZE_T dwSize,
	_In_     DWORD flAllocationType,
	_In_     DWORD flProtect);

RTL_VERIFIER_THUNK_DESCRIPTOR funcs[]{
	{ (PCHAR)"VirtualAlloc", nullptr, HookVirtualAlloc },
	{ nullptr, nullptr, nullptr },
};

RTL_VERIFIER_DLL_DESCRIPTOR dlls[] = {
	{ (PWSTR)L"Kernel32.dll", 0, nullptr, funcs },
	{ nullptr, 0, nullptr, nullptr }
};

RTL_VERIFIER_PROVIDER_DESCRIPTOR desc{
	sizeof(desc),
	dlls,
	[](auto, auto, auto, auto) {},
	[](auto, auto, auto, auto) {},
	nullptr, 0, 0,
	nullptr, nullptr, nullptr,
	[](auto, auto) {}
};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			// 
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;

		case DLL_PROCESS_VERIFIER:
			*(PVOID*)lpReserved = &desc;
			//
			break;
	}
	return TRUE;
}

LPVOID WINAPI HookVirtualAlloc(
	_In_opt_ LPVOID lpAddress,
	_In_     SIZE_T dwSize,
	_In_     DWORD flAllocationType,
	_In_     DWORD flProtect) {
	OutputDebugStringA(std::format("TID: {} Address: {:p} Size: {} Type: {} Protect: {}",
		GetCurrentThreadId(), lpAddress, dwSize, flAllocationType, flProtect).c_str());
	static const auto org = (decltype(VirtualAlloc)*)funcs[0].ThunkOldAddress;
	auto p = org(lpAddress, dwSize, flAllocationType, flProtect);
	OutputDebugStringA(std::format(" Result: {:p}\n", p).c_str());

	return p;
}

