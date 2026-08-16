// MemMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <string>
#include <winternl.h>
#include <format>

#pragma comment(lib, "ntdll")

const char* GetBlockState(DWORD state) {
	switch (state) {
		case MEM_FREE: return "Free";
		case MEM_COMMIT: return "Committed";
		case MEM_RESERVE: return "Reserved";
	}
	return "";
}

const char* GetBlockType(DWORD type) {
	switch (type) {
		case MEM_IMAGE: return "Image";
		case MEM_MAPPED: return "Mapped";
		case MEM_PRIVATE: return "Private";
	}
	return "";
}

std::string GetBlockProtection(DWORD prot) {
	std::string s;
	switch (prot & 0xff) {
		case PAGE_NOACCESS: s = "---"; break;
		case PAGE_READONLY: s = "R--"; break;
		case PAGE_READWRITE: s = "RW-"; break;
		case PAGE_EXECUTE_READ: s = "R-X"; break;
		case PAGE_EXECUTE_READWRITE: s = "RWX"; break;
		case PAGE_WRITECOPY: s = "-W-C"; break;
		case PAGE_EXECUTE_WRITECOPY: s = "-WXC"; break;
	}
	if (prot & PAGE_GUARD)
		s += "/G";
	if (prot & PAGE_WRITECOMBINE)
		s += "/WC";

	return s;
}

std::string GetBlockDetails(HANDLE hProcess, MEMORY_BASIC_INFORMATION const& mbi) {
	if (mbi.State != MEM_COMMIT)
		return "";

	std::string details;
	PROCESS_BASIC_INFORMATION pbi;
	if (0 == NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), nullptr)) {
		if (pbi.PebBaseAddress >= mbi.BaseAddress && (PBYTE)pbi.PebBaseAddress < (PBYTE)mbi.BaseAddress + mbi.RegionSize) {
			details = std::format("PEB (0x{})", (PVOID)pbi.PebBaseAddress);
		}
	}

	return details;
}

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: memmap <pid>\n");
		return 0;
	}

	auto pid = strtoul(argv[1], nullptr, 0);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
	if (!hProcess) {
		printf("Error: %u\n", GetLastError());
		return 1;
	}

	PBYTE address = nullptr;

	MEMORY_BASIC_INFORMATION mbi;
	while (VirtualQueryEx(hProcess, address, &mbi, sizeof(mbi))) {
		printf("%s 0x%p-0x%p (%10llu KB) %-8s %-8s %-8s %s\n",
			mbi.BaseAddress == mbi.AllocationBase ? "*" : " ",
			mbi.BaseAddress, (PBYTE)mbi.BaseAddress + mbi.RegionSize, mbi.RegionSize >> 10,
			GetBlockState(mbi.State), GetBlockType(mbi.Type), 
			GetBlockProtection(mbi.Protect).c_str(),
			GetBlockDetails(hProcess, mbi).c_str());
		address += mbi.RegionSize;
	}
	CloseHandle(hProcess);
	return 0;
}

