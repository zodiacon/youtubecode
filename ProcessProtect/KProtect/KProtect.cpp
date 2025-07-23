#include <ntddk.h>

extern "C" 
NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING /*RegistryPath*/) {
	UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\KProtect");
	PDEVICE_OBJECT devObj;
	NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName,
		FILE_DEVICE_UNKNOWN, 0, FALSE, &devObj);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Error in IoCreateDevice (0x%X)\n", status);
		return status;
	}

	UNICODE_STRING symName = RTL_CONSTANT_STRING(L"\\??\\KProtect");
	status = IoCreateSymbolicLink(&symName, &devName);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Error in IoCreateSymbolicLink (0x%X)\n", status);
		IoDeleteDevice(devObj);
		return status;
	}

	return status;
}
