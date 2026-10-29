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
#include <assert.h>
#include <string>

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

void* GetProcessPeb(HANDLE hProcess) {
	PROCESS_BASIC_INFORMATION pbi{};
	NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), nullptr);
	return pbi.PebBaseAddress;
}

bool Write(HANDLE hProcess, void* address, void const* data, DWORD size) {
	SIZE_T written;
	return WriteProcessMemory(hProcess, address, data, size, &written) && written == size;
}

bool Read(HANDLE hProcess, void* address, void* data, DWORD size) {
	SIZE_T read;
	return ReadProcessMemory(hProcess, address, data, size, &read) && read == size;
}

template<typename T>
T Read(HANDLE hProcess, void* address) {
	T value;
	auto b = Read(hProcess, address, &value, sizeof(value));
	assert(b);
	return value;
}

std::wstring ReadUnicodeString(HANDLE hProcess, UNICODE_STRING const& uc) {
	std::wstring text;
	text.resize(uc.Length / sizeof(WCHAR));
	Read(hProcess, uc.Buffer, text.data(), uc.Length);
	return text;
}

bool EnumDllsWithPeb(HANDLE hProcess) {
	auto pebAddress = GetProcessPeb(hProcess);
	PEB peb = Read<PEB>(hProcess, pebAddress);
	auto ldr = Read<PEB_LDR_DATA>(hProcess, peb.Ldr);
	auto head = (LIST_ENTRY*)((PBYTE)peb.Ldr + offsetof(PEB_LDR_DATA, InMemoryOrderModuleList));

	LIST_ENTRY next = Read<LIST_ENTRY>(hProcess, head);
	for (; next.Flink != head; next = Read<LIST_ENTRY>(hProcess, next.Flink)) {
		LDR_DATA_TABLE_ENTRY* entryAddress = CONTAINING_RECORD(next.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		auto entry = Read<LDR_DATA_TABLE_ENTRY>(hProcess, entryAddress);
		printf("0x%p: %ws\n", entry.DllBase, ReadUnicodeString(hProcess, entry.FullDllName).c_str());
	}
	return true;
}

bool HideDll(HANDLE hProcess, PCWSTR name) {
	auto pebAddress = GetProcessPeb(hProcess);
	PEB peb = Read<PEB>(hProcess, pebAddress);
	auto ldr = Read<PEB_LDR_DATA>(hProcess, peb.Ldr);
	auto head = (LIST_ENTRY*)((PBYTE)peb.Ldr + offsetof(PEB_LDR_DATA, InMemoryOrderModuleList));

	LIST_ENTRY next = Read<LIST_ENTRY>(hProcess, head);
	for (; next.Flink != head; next = Read<LIST_ENTRY>(hProcess, next.Flink)) {
		LDR_DATA_TABLE_ENTRY* entryAddress = CONTAINING_RECORD(next.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		auto entry = Read<LDR_DATA_TABLE_ENTRY>(hProcess, entryAddress);
		if (_wcsicmp(ReadUnicodeString(hProcess, entry.BaseDllName).c_str(), name) == 0) {
			auto current = entry.InMemoryOrderLinks;
			Write(hProcess, current.Blink, &current.Flink, sizeof(LIST_ENTRY*));
			Write(hProcess, (PBYTE)current.Flink + sizeof(PVOID), &current.Blink, sizeof(LIST_ENTRY*));
			return true;
		}
	}
	return false;
}

int main(int argc, const char* argv[]) {
	auto pid = argc > 1 ? strtoul(argv[1], nullptr, 0) : GetCurrentProcessId();
	auto hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (!hProcess) {
		printf("Failed in OpenProcess (%u)\n", GetLastError());
		return 1;
	}

	EnumDllsWithAPI(hProcess);
	printf("\n");
	EnumDllsWithPeb(hProcess);

	HideDll(hProcess, L"kernelbase.dll");
	printf("\n");

	EnumDllsWithAPI(hProcess);
	printf("\n");
	EnumDllsWithPeb(hProcess);

	CloseHandle(hProcess);

	return 0;
}
