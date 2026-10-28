// HideDll.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define NOMINMAX
#include <phnt_windows.h>
#include <phnt.h>
#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <utility>
//#include <winternl.h>

#pragma comment(lib, "ntdll")

bool EnumDllsWithAPI(HANDLE hProcess) {
	HMODULE h[1024];
	DWORD needed;
	if (!EnumProcessModules(hProcess, h, sizeof(h), &needed))
		return false;

	WCHAR name[256];
	for (int i = 0; i < std::min(needed / sizeof(HMODULE), _countof(h)); i++) {
		GetModuleBaseName(hProcess, h[i], name, _countof(name));
		printf("0x%p: %ws\n", h[i], name);
	}
	return true;
}

bool EnumDllsWithPeb(HANDLE) {
	auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;
	auto head = &peb->Ldr->InMemoryOrderModuleList;

	for (LIST_ENTRY* next = head->Flink; next != head; next = next->Flink) {
		LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(next, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		printf("0x%p: %wZ\n", entry->DllBase, &entry->FullDllName);
	}
	return true;
}

bool HideDll(PCWSTR name) {
	UNICODE_STRING uname;
	RtlInitUnicodeString(&uname, name);
	auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;
	auto head = &peb->Ldr->InMemoryOrderModuleList;

	for (LIST_ENTRY* next = head->Flink; next != head; next = next->Flink) {
		LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(next, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		if (RtlEqualUnicodeString(&entry->BaseDllName, &uname, TRUE)) {
			auto prev = next->Blink;
			prev->Flink = next->Flink;
			next->Flink->Blink = prev;
			return true;
		}
	}
	return false;
}

int main() {
	EnumDllsWithAPI(GetCurrentProcess());
	printf("\n");
	EnumDllsWithPeb(GetCurrentProcess());

	HideDll(L"kernelbase.dll");
	printf("\n");

	EnumDllsWithAPI(GetCurrentProcess());
	printf("\n");
	EnumDllsWithPeb(GetCurrentProcess());

	return 0;
}
