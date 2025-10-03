// EnumWinWithNative.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <phnt_windows.h>
#include <phnt.h>
#include <stdio.h>
#include <string>
#include <vector>

#pragma comment(lib, "ntdll")

std::vector<std::wstring> GetWindowStations(DWORD id) {
	HANDLE hDirectory;
	OBJECT_ATTRIBUTES dirAttr;
	UNICODE_STRING path;
	WCHAR spath[128];
	if (id == 0) {
		RtlInitUnicodeString(&path, L"\\Windows\\WindowStations");
	}
	else {
		swprintf_s(spath, L"\\Sessions\\%u\\Windows\\WindowStations", id);
		RtlInitUnicodeString(&path, spath);
	}
	InitializeObjectAttributes(&dirAttr, &path, 0, nullptr, nullptr);
	auto status = NtOpenDirectoryObject(&hDirectory, DIRECTORY_QUERY | DIRECTORY_TRAVERSE, 
		&dirAttr);
	if (!NT_SUCCESS(status)) {
		return {};
	}

	UNICODE_STRING winStationTypeName;
	RtlInitUnicodeString(&winStationTypeName, L"WindowStation");

	std::vector<std::wstring> items;
	BYTE buffer[1 << 10];
	for (ULONG i = 0;;) {
		status = NtQueryDirectoryObject(hDirectory, buffer, sizeof(buffer),
			TRUE, FALSE, &i, nullptr);
		if (!NT_SUCCESS(status))
			break;

		auto dir = (POBJECT_DIRECTORY_INFORMATION)buffer;
		if (RtlEqualUnicodeString(&dir->TypeName, &winStationTypeName, FALSE)) {
			items.push_back(std::wstring(dir->Name.Buffer, dir->Name.Length / sizeof(WCHAR)));
		}
	}
	NtClose(hDirectory);
	return items;
}

int main() {
	for (DWORD i = 0; i < 256; i++) {
		auto stations = GetWindowStations(i);
		if (!stations.empty()) {
			printf("Session %u\n", i);
			for (auto& name : stations)
				printf("  %ws\n", name.c_str());
		}
	}
	return 0;
}
