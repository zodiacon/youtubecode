#include <ntifs.h>

void OnUnload(PDRIVER_OBJECT DriverObject);
void OnProcessNotify(
	_Inout_ PEPROCESS Process,
	_In_ HANDLE ProcessId,
	_Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);
NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING /*RegistryPath*/) {
	//	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrint("KProcNotify DriverEntry called (0x%p)\n", DriverObject);

	DriverObject->DriverUnload = OnUnload;

	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, L"\\Device\\KProcNotify");
	PDEVICE_OBJECT devObj;
	auto status = IoCreateDevice(DriverObject, 0, &devName,
		FILE_DEVICE_UNKNOWN, 0, TRUE, &devObj);
	if (!NT_SUCCESS(status)) {
		DbgPrint("KProcNotify: Error in IoCreateDevice (0x%X)\n",
			status);
		return status;
	}

	UNICODE_STRING symName = RTL_CONSTANT_STRING(L"\\??\\ProcNotify");
	status = IoCreateSymbolicLink(&symName, &devName);
	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(devObj);
		DbgPrint("KProcNotify: Error in IoCreateSymbolicLink (0x%X)\n",
			status);
		return status;
	}

	status = PsSetCreateProcessNotifyRoutineEx(OnProcessNotify, FALSE);
	if (!NT_SUCCESS(status)) {
		DbgPrint("KProcNotify: Error in PsSetCreateProcessNotifyRoutineEx (0x%X)\n",
			status);
		IoDeleteSymbolicLink(&symName);
		IoDeleteDevice(devObj);
		return status;
	}

	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverObject->MajorFunction[IRP_MJ_CLOSE] = 
		OnCreateClose;

	return STATUS_SUCCESS;
}

void OnUnload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("KProcNotify unload called (0x%p)\n", DriverObject);
	PsSetCreateProcessNotifyRoutineEx(OnProcessNotify, TRUE);
	UNICODE_STRING symName = RTL_CONSTANT_STRING(L"\\??\\ProcNotify");
	IoDeleteSymbolicLink(&symName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

void OnProcessNotify(PEPROCESS Process, HANDLE ProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo) {
	if (CreateInfo) {
		DbgPrint("KProcNotify Create: 0x%p PID: %lu Image: %wZ\n",
			Process, HandleToULong(ProcessId), CreateInfo->ImageFileName);
	}
	else {
		auto exitCode = PsGetProcessExitStatus(Process);
		DbgPrint("KProcNotify Exit: 0x%p PID: %lu Exit code: 0x%lX\n",
			Process, HandleToULong(ProcessId), exitCode);
	}
}

NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp) {
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}
