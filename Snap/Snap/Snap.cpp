// Snap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <ProcessSnapshot.h>
#include <string>

void DisplayProcessInfo(HPSS hSnapshot) {
	PSS_PROCESS_INFORMATION info;
	if (ERROR_SUCCESS == PssQuerySnapshot(hSnapshot, PSS_QUERY_PROCESS_INFORMATION, &info, sizeof(info))) {
		printf("Image file: %ws\n", info.ImageFileName);
		printf("PID: %u\n", info.ProcessId);
		printf("PPID: %u\n", info.ParentProcessId);
	}
}

void DisplayHandlesInfo(HPSS hSnapshot) {
	PSS_HANDLE_INFORMATION info;
	if (ERROR_SUCCESS == PssQuerySnapshot(hSnapshot, PSS_QUERY_HANDLE_INFORMATION, &info, sizeof(info))) {
		printf("Handles: %u\n", info.HandlesCaptured);
	}

	HPSSWALK hWalk;
	if (ERROR_SUCCESS == PssWalkMarkerCreate(nullptr, &hWalk)) {
		PSS_HANDLE_ENTRY entry;
		while (ERROR_SUCCESS == PssWalkSnapshot(hSnapshot, PSS_WALK_HANDLES, hWalk, &entry, sizeof(entry))) {
			if (entry.ObjectNameLength > 0) {
				printf("Handle: %u", (DWORD)(DWORD_PTR)entry.Handle);
				printf(" Name: %ws",
					std::wstring(entry.ObjectName, entry.ObjectNameLength / sizeof(WCHAR)).c_str());
				printf("\n");
			}
		}
		PssWalkMarkerFree(hWalk);
	}
}

int main(int argc, const char* argv[]) {
	if (argc < 2) {
		printf("Usage: snap <pid>\n");
		return 0;
	}

	auto pid = strtoul(argv[1], nullptr, 0);
	HANDLE hProcess = OpenProcess(MAXIMUM_ALLOWED, FALSE, pid);
	if (!hProcess)
		return 1;

	HPSS hSnapshot;
	if (ERROR_SUCCESS != PssCaptureSnapshot(hProcess, PSS_CAPTURE_HANDLES | PSS_CAPTURE_THREADS |
		PSS_CAPTURE_HANDLE_NAME_INFORMATION | PSS_CAPTURE_HANDLE_BASIC_INFORMATION |
		PSS_CAPTURE_VA_SPACE | PSS_CAPTURE_VA_SPACE_SECTION_INFORMATION | PSS_CAPTURE_VA_CLONE,
		0, &hSnapshot))
		return 1;

	DisplayProcessInfo(hSnapshot);
	//DisplayThreadInfo(hSnapshot);
	DisplayHandlesInfo(hSnapshot);

	PssFreeSnapshot(hProcess, hSnapshot);
	CloseHandle(hProcess);

	return 0;
}
