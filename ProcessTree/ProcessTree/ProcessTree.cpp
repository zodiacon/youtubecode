// ProcessTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;

struct ProcessInfo {
	DWORD Id;
	DWORD ParentId;
	wstring Name;
	vector<unique_ptr<ProcessInfo>> Children;
};

void DisplayTree(vector<unique_ptr<ProcessInfo>> const& roots, int indent = 0) {
	for (auto& pi : roots) {
		printf("%s", string(indent, ' ').c_str());
		printf("%ws (%u)\n", pi->Name.c_str(), pi->Id);
		DisplayTree(pi->Children, indent + 1);
	}
}

ULONG64 GetStartTime(DWORD pid) {
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
	if (!hProcess)
		return 0;

	FILETIME start, dummy;
	GetProcessTimes(hProcess, &start, &dummy, &dummy, &dummy);
	CloseHandle(hProcess);
	return *(ULONG64*)&start;
}

int main() {
	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 1;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	vector<unique_ptr<ProcessInfo>> roots;
	unordered_map<DWORD, ProcessInfo*> map;

	Process32First(hSnapshot, &pe);
	do {
		auto pi = make_unique<ProcessInfo>();
		pi->Id = pe.th32ProcessID;
		pi->ParentId = pe.th32ParentProcessID;
		pi->Name = pe.szExeFile;

		auto p = pi.get();

		bool root = true;
		if (auto it = map.find(pi->ParentId); it != map.end()) {
			auto startParent = GetStartTime(pi->ParentId);
			auto startChild = GetStartTime(pi->Id);
			if (startParent && startChild && startParent < startChild) {
				it->second->Children.push_back(move(pi));
				root = false;
			}
		}
		if(root) {
			roots.push_back(move(pi));
		}
		map.insert({ p->Id, p });
	} while (Process32Next(hSnapshot, &pe));

	CloseHandle(hSnapshot);

	DisplayTree(roots);
	return 0;
}
