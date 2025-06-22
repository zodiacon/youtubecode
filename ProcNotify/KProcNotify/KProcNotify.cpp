#include <ntifs.h>
#include "ProcNotifyShared.h"

#define MEMORY_TAG 'oNrP'

const int MaxEventCount = 1000;
EventHeader* g_Events[MaxEventCount];
int g_NextToWrite = 0, g_NextToRead = 0;
int g_EventCount;

void OnUnload(PDRIVER_OBJECT DriverObject);
void OnProcessNotify(
	_Inout_ PEPROCESS Process,
	_In_ HANDLE ProcessId,
	_Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);
NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp);
NTSTATUS OnRead(PDEVICE_OBJECT, PIRP Irp);

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
	DriverObject->MajorFunction[IRP_MJ_READ] = OnRead;

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
	if (g_EventCount >= MaxEventCount) {
		return;
	}

	if (CreateInfo) {
		DbgPrint("KProcNotify Create: 0x%p PID: %lu Image: %wZ\n",
			Process, HandleToULong(ProcessId), CreateInfo->ImageFileName);

		ULONG size = sizeof(ProcessCreate);
		if (CreateInfo->CommandLine)
			size += CreateInfo->CommandLine->Length;
		auto evt = (ProcessCreate*)ExAllocatePool2(POOL_FLAG_PAGED, size, MEMORY_TAG);
		if (!evt) {
			DbgPrint("KProcNotify: Out of memory!");
			return;
		}

		evt->Size = size;
		evt->ProcessId = HandleToULong(ProcessId);
		KeQuerySystemTimePrecise((PLARGE_INTEGER)&evt->Time);
		evt->Type = EventType::ProcessCreate;
		evt->ParentId = HandleToULong(CreateInfo->ParentProcessId);
		if (CreateInfo->CommandLine)
			memcpy(evt->CommandLine, CreateInfo->CommandLine->Buffer, CreateInfo->CommandLine->Length);
		g_Events[g_NextToWrite++] = evt;
		g_EventCount++;
		if (g_NextToWrite == MaxEventCount)
			g_NextToWrite = 0;
	}
	else {
		auto exitCode = PsGetProcessExitStatus(Process);
		DbgPrint("KProcNotify Exit: 0x%p PID: %lu Exit code: 0x%lX\n",
			Process, HandleToULong(ProcessId), exitCode);

		auto evt = (ProcessExit*)ExAllocatePool2(POOL_FLAG_PAGED, sizeof(ProcessExit), MEMORY_TAG);
		if (!evt) {
			DbgPrint("KProcNotify: Out of memory!");
			return;
		}

		evt->ProcessId = HandleToULong(ProcessId);
		KeQuerySystemTimePrecise((PLARGE_INTEGER)&evt->Time);
		evt->Size = sizeof(ProcessExit);
		evt->Type = EventType::ProcessExit;
		evt->ExitCode = exitCode;
		g_Events[g_NextToWrite++] = evt;
		g_EventCount++;
		if (g_NextToWrite == MaxEventCount)
			g_NextToWrite = 0;
	}
}

NTSTATUS OnCreateClose(PDEVICE_OBJECT, PIRP Irp) {
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}

NTSTATUS OnRead(PDEVICE_OBJECT, PIRP Irp) {
	ULONG len = 0;
	if (g_EventCount) {
		PIO_STACK_LOCATION irpSp = IoGetCurrentIrpStackLocation(Irp);
		auto userLen = irpSp->Parameters.Read.Length;
		auto buffer = (PUCHAR)Irp->UserBuffer;
		while (g_EventCount > 0) {
			auto evt = g_Events[g_NextToRead];
			if (userLen < evt->Size)
				break;

			memcpy(buffer, evt, evt->Size);
			if (++g_NextToRead == MaxEventCount)
				g_NextToRead = 0;
			userLen -= evt->Size;
			buffer += evt->Size;
			g_EventCount--;
			len += evt->Size;
			ExFreePool(evt);
		}
	}

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = len;
	IoCompleteRequest(Irp, 0);
	return STATUS_SUCCESS;
}
