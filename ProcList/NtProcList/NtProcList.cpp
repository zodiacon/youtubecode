// NtProcList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <phnt_windows.h>
#include <phnt.h>
#include <stdio.h>
#include <memory>

#pragma comment(lib, "ntdll")

int main() {
	ULONG size;
	std::unique_ptr<BYTE[]> buffer;

	auto status = NtQuerySystemInformation(SystemProcessInformation,
		nullptr, 0, &size);
	while (status == STATUS_INFO_LENGTH_MISMATCH) {
		size += 4096;
		buffer = std::make_unique<BYTE[]>(size);
		status = NtQuerySystemInformation(SystemProcessInformation,
			buffer.get(), size, &size);
	}

	if (!NT_SUCCESS(status))
		return 1;

	auto p = (SYSTEM_PROCESS_INFORMATION*)buffer.get();
	for (;;) {
		printf("PID: %6u PPID: %6u T: %3u H: %6u S: %u Name: %wZ\n",
			HandleToULong(p->UniqueProcessId),
			HandleToULong(p->InheritedFromUniqueProcessId),
			p->NumberOfThreads,
			p->HandleCount,
			p->SessionId,
			&p->ImageName);

		if (p->NextEntryOffset == 0)
			break;

		p = (SYSTEM_PROCESS_INFORMATION*)((PBYTE)p + p->NextEntryOffset);
	}
	return 0;
}

