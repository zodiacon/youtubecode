// MemMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <phnt_windows.h>
#include <phnt.h>
#include <stdio.h>
#include <string>
#include <format>
#include <Psapi.h>
#include <TlHelp32.h>
#include <map>
#include <vector>

#pragma comment(lib, "ntdll")

struct ThreadInfo {
	DWORD Id;
	PVOID Teb;
	PVOID StackStart, StackEnd;
};

ThreadInfo GetThreadInfo(DWORD id, HANDLE hProcess) {
	ThreadInfo ti{};
	ti.Id = id;

	if (auto hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, id); hThread != nullptr) {
		THREAD_BASIC_INFORMATION tbi;
		if (0 == NtQueryInformationThread(hThread, ThreadBasicInformation, &tbi, sizeof(tbi), nullptr)) {
			ti.Teb = tbi.TebBaseAddress;
			NT_TIB tib;
			if (ReadProcessMemory(hProcess, ti.Teb, &tib, sizeof(tib), nullptr)) {
				ti.StackStart = tib.StackBase;
				ti.StackEnd = tib.StackLimit;
			}
		}
		CloseHandle(hThread);
	}
	return ti;
}

std::vector<ThreadInfo> EnumThreads(HANDLE hProcess) {
	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return {};

	THREADENTRY32 te;
	te.dwSize = sizeof(te);
	Thread32First(hSnapshot, &te);
	auto pid = GetProcessId(hProcess);

	std::vector<ThreadInfo> threads;
	while (Thread32Next(hSnapshot, &te)) {
		if (te.th32OwnerProcessID == pid) {
			threads.push_back(GetThreadInfo(te.th32ThreadID, hProcess));
		}
	}

	return threads;
}

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

std::string NtPathToDosPath(const char* path) {
	if (path[0] != '\\')
		return path;

	static std::map<std::string, std::string> driveMaps;
	if (driveMaps.empty()) {
		DWORD drives = GetLogicalDrives();
		char drive[] = "X:";
		int bit = 0;
		// 000000000001100
		while (drives) {
			char target[128];
			if (drives & 1) {
				drive[0] = 'A' + bit;
				if (QueryDosDeviceA(drive, target, _countof(target))) {
					driveMaps.insert({ target, drive });
				}
			}
			bit++;
			drives >>= 1;
		}
	}

	// \Device\HardiskVolume3\ -> c:
	auto bs = strchr(path + 9, '\\');
	std::string ntdrive = std::string(path, bs);

	if (auto it = driveMaps.find(ntdrive); it == driveMaps.end())
		return path;
	else
		return std::string(it->second) + bs;
}

std::string GetBlockDetails(HANDLE hProcess, MEMORY_BASIC_INFORMATION const& mbi) {
	if (mbi.State != MEM_COMMIT)
		return "";

	static auto threads = EnumThreads(hProcess);

	std::string details;
	if (mbi.Type == MEM_PRIVATE) {
		static PROCESS_BASIC_INFORMATION pbi;
		if (pbi.PebBaseAddress == nullptr) {
			NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), nullptr);
		}
		if (pbi.PebBaseAddress && pbi.PebBaseAddress >= mbi.BaseAddress && (PBYTE)pbi.PebBaseAddress < (PBYTE)mbi.BaseAddress + mbi.RegionSize) {
			details = std::format("PEB (0x{})", (PVOID)pbi.PebBaseAddress);
			return details;
		}

		for (auto& t : threads) {
			if (t.Teb && mbi.BaseAddress <= t.Teb && t.Teb < (PBYTE)mbi.BaseAddress + mbi.RegionSize) {
				return std::format("TEB (0x{}) ID: {}", t.Teb, t.Id);
			}
			if (t.StackEnd && mbi.BaseAddress >= t.StackEnd && mbi.BaseAddress < t.StackStart) {
				return std::format("Stack ID: {}", t.Id);
			}
		}
	}
	else {
		char path[MAX_PATH];
		if (GetMappedFileNameA(hProcess, mbi.BaseAddress, path, _countof(path)))
			return NtPathToDosPath(path);
	}
	return details;
}

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: memmap <pid>\n");
		return 0;
	}

	auto pid = strtoul(argv[1], nullptr, 0);
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
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

