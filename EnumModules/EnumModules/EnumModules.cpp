// EnumModules.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <Psapi.h>
#include <TlHelp32.h>
#include <string>
#include <vector>

struct ModuleInfo {
	HMODULE Address;
	std::wstring Name;
};

std::vector<ModuleInfo> EnumModules3(DWORD pid) {
	auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (!hProcess)
		return {};

	PBYTE address = nullptr;
	MEMORY_BASIC_INFORMATION mbi;
	WCHAR name[MAX_PATH];
	std::vector<ModuleInfo> modules;
	while (VirtualQueryEx(hProcess, address, &mbi, sizeof(mbi))) {
		if (mbi.AllocationBase == mbi.BaseAddress) {
			if (mbi.State == MEM_COMMIT && mbi.Type != MEM_PRIVATE) {
				if (GetMappedFileName(hProcess, mbi.AllocationBase, name, _countof(name))) {
					ModuleInfo mi;
					mi.Address = (HMODULE)mbi.AllocationBase;
					mi.Name = wcsrchr(name, L'\\') + 1;
					modules.push_back(std::move(mi));
				}
			}
		}
		address += mbi.RegionSize;
	}
	CloseHandle(hProcess);
	return modules;
}

std::vector<ModuleInfo> EnumModules2(DWORD pid) {
	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return {};

	MODULEENTRY32 me;
	me.dwSize = sizeof(me);

	std::vector<ModuleInfo> modules;
	Module32First(hSnapshot, &me);
	do {
		ModuleInfo mi;
		mi.Address = me.hModule;
		mi.Name = me.szModule;
		modules.push_back(std::move(mi));
	} while (Module32Next(hSnapshot, &me));

	CloseHandle(hSnapshot);
	return modules;
}


std::vector<ModuleInfo> EnumModules1(DWORD pid) {
	auto hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ, FALSE, pid);
	if (!hProcess)
		return {};

	HMODULE hModule[1024];
	DWORD needed;
	WCHAR name[256];
	std::vector<ModuleInfo> modules;
	if (EnumProcessModules(hProcess, hModule, sizeof(hModule), &needed)) {
		for (DWORD i = 0; i < needed / sizeof(HMODULE); i++) {
			ModuleInfo mi;
			mi.Address = hModule[i];
			if (GetModuleBaseName(hProcess, hModule[i], name, _countof(name))) {
				mi.Name = name;
			}
			modules.push_back(std::move(mi));
		}
	}
	CloseHandle(hProcess);
	return modules;
}

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: EmumModules <pid>\n");
		return 0;
	}

	DWORD pid = strtoul(argv[1], nullptr, 0);
	printf("Method 1\n");
	auto modules = EnumModules1(pid);
	if (modules.empty()) {
		printf("Error! (%u)\n", GetLastError());
		return 1;
	}

	for (auto& m : modules) {
		printf("Address: 0x%p Name: %ws\n",
			m.Address, m.Name.c_str());
	}
	printf("Method 2\n");
	modules = EnumModules2(pid);
	if (modules.empty()) {
		printf("Error! (%u)\n", GetLastError());
		return 1;
	}

	for (auto& m : modules) {
		printf("Address: 0x%p Name: %ws\n",
			m.Address, m.Name.c_str());
	}

	printf("Method 3\n");
	modules = EnumModules3(pid);
	if (modules.empty()) {
		printf("Error! (%u)\n", GetLastError());
		return 1;
	}

	for (auto& m : modules) {
		printf("Address: 0x%p Name: %ws\n",
			m.Address, m.Name.c_str());
	}


	return 0;
}

