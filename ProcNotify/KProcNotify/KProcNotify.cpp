#include <ntifs.h>

void OnUnload(PDRIVER_OBJECT DriverObject);
void OnProcessNotify(
	_Inout_ PEPROCESS Process,
	_In_ HANDLE ProcessId,
	_Inout_opt_ PPS_CREATE_NOTIFY_INFO CreateInfo);

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING /*RegistryPath*/) {
	//	UNREFERENCED_PARAMETER(DriverObject);

	DbgPrint("KProcNotify DriverEntry called (0x%p)\n", DriverObject);

	DriverObject->DriverUnload = OnUnload;

	auto status = PsSetCreateProcessNotifyRoutineEx(OnProcessNotify, FALSE);
	if (!NT_SUCCESS(status)) {
		DbgPrint("KProcNotify: Error in PsSetCreateProcessNotifyRoutineEx (0x%X)\n",
			status);
		return status;
	}
	return STATUS_SUCCESS;
}

void OnUnload(PDRIVER_OBJECT DriverObject) {
	DbgPrint("KProcNotify unload called (0x%p)\n", DriverObject);
	PsSetCreateProcessNotifyRoutineEx(OnProcessNotify, TRUE);
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
