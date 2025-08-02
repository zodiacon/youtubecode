#include <ntddk.h>

PVOID g_RegHandle;
ULONG g_ProtectId;

void OnUnload(PDRIVER_OBJECT DriverObject);
NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp);
NTSTATUS OnWrite(PDEVICE_OBJECT, PIRP Irp);
void OnPostOpenProcess(
	_In_ PVOID RegistrationContext,
	_In_ POB_POST_OPERATION_INFORMATION OperationInformation);

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

	OB_OPERATION_REGISTRATION typeReg{
		PsProcessType,
		OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE,
		nullptr,
		OnPostOpenProcess,
	};

	OB_CALLBACK_REGISTRATION reg{
		OB_FLT_REGISTRATION_VERSION,
		1,
		RTL_CONSTANT_STRING(L"123456.345"),
		nullptr,
		&typeReg,
	};

	status = ObRegisterCallbacks(&reg, &g_RegHandle);
	if (!NT_SUCCESS(status)) {
		DbgPrint("Error in ObRegisterCallbacks (0x%X)\n", status);
		IoDeleteSymbolicLink(&symName);
		IoDeleteDevice(devObj);
		return status;
	}

	DriverObject->DriverUnload = OnUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverObject->MajorFunction[IRP_MJ_CLOSE] =
		OnCreateClose;

	DriverObject->MajorFunction[IRP_MJ_WRITE] = OnWrite;

	return status;
}

void OnUnload(PDRIVER_OBJECT DriverObject) {
	ObUnRegisterCallbacks(g_RegHandle);
	UNICODE_STRING symName = RTL_CONSTANT_STRING(L"\\??\\KProtect");
	IoDeleteSymbolicLink(&symName);
	IoDeleteDevice(DriverObject->DeviceObject);
}

NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp) {
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}

NTSTATUS OnWrite(PDEVICE_OBJECT, PIRP Irp) {
	auto status = STATUS_SUCCESS;
	auto irpSp = IoGetCurrentIrpStackLocation(Irp);
	ULONG len = 0;
	if (irpSp->Parameters.Write.Length < sizeof(ULONG)) {
		status = STATUS_BUFFER_TOO_SMALL;
	}
	else {
		g_ProtectId = *(ULONG*)Irp->UserBuffer;
		len = sizeof(ULONG);
	}

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = len;
	IoCompleteRequest(Irp, 0);
	return status;
}

void OnPostOpenProcess(PVOID, POB_POST_OPERATION_INFORMATION info) {
	UNREFERENCED_PARAMETER(info);
}
