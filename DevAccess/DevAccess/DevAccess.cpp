// DevAccess.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <ntddbeep.h>
#include <stdio.h>
#include <winternl.h>

#pragma comment(lib, "ntdll")

int main() {
	//HANDLE hDevice = CreateFile(LR"(\\.\GlobalRoot\Device\Beep)",
	//	GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
	//	0, nullptr);
	HANDLE hDevice;
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, DD_BEEP_DEVICE_NAME_U);
	OBJECT_ATTRIBUTES devAttr;
	InitializeObjectAttributes(&devAttr, &devName, 0, nullptr, nullptr);
	IO_STATUS_BLOCK ioStatus;
	NTSTATUS status = NtOpenFile(&hDevice, GENERIC_WRITE,
		&devAttr, &ioStatus, 0, 0);

	if (status == 0) {
		printf("handle: 0x%p\n", hDevice);

		BEEP_SET_PARAMETERS note;
		note.Frequency = 800;		// Hz
		note.Duration = 2000;		// msec
		DWORD ret;
		DeviceIoControl(hDevice, IOCTL_BEEP_SET,
			&note, sizeof(note), nullptr, 0, &ret, nullptr);

		CloseHandle(hDevice);
	}

	return 0;
}
