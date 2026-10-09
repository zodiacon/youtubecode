// ObjAttr.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <phnt_Windows.h>
#include <phnt.h>

#pragma comment(lib, "ntdll")

int main() {
	OBJECT_ATTRIBUTES dirAttr;
	HANDLE hDevicesDir;
	UNICODE_STRING dirName;
	RtlInitUnicodeString(&dirName, L"\\Device");
	InitializeObjectAttributes(&dirAttr, &dirName, OBJ_CASE_INSENSITIVE, nullptr, nullptr);
	auto status = NtOpenDirectoryObject(&hDevicesDir, DIRECTORY_QUERY, &dirAttr);
	if (status == STATUS_SUCCESS) {
		HANDLE hDevice;
		OBJECT_ATTRIBUTES fileAttr;
		UNICODE_STRING name;
		RtlInitUnicodeString(&name, L"Beep");
		InitializeObjectAttributes(&fileAttr, &name, OBJ_CASE_INSENSITIVE, hDevicesDir, nullptr);
		IO_STATUS_BLOCK ioStatus;
		status = NtOpenFile(&hDevice, GENERIC_WRITE, &fileAttr, &ioStatus, 0, 0);
		if (status == STATUS_SUCCESS) {
			NtClose(hDevice);
		}
		NtClose(hDevicesDir);
	}
	return 0;
}
