// Iocp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

HANDLE hJob;

DWORD WINAPI MonitorJob(PVOID p) {
	HANDLE hCP = (HANDLE)p;
	for (;;) {
		ULONG_PTR key;
		DWORD msg;
		LPOVERLAPPED ov;
		if (!GetQueuedCompletionStatus(hCP, &msg, &key, &ov, INFINITE))
			break;

		if (msg == JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT) {
			printf("Active limit reached... increasing\n");
			JOBOBJECT_BASIC_ACCOUNTING_INFORMATION info;
			QueryInformationJobObject(hJob, JobObjectBasicAccountingInformation,
				&info, sizeof(info), nullptr);

			JOBOBJECT_BASIC_LIMIT_INFORMATION linfo{};
			linfo.ActiveProcessLimit = info.ActiveProcesses + 5;
			linfo.LimitFlags = JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
			SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &linfo, sizeof(linfo));

		}
	}
	return 0;
}

int main() {
	hJob = CreateJobObject(nullptr, L"IocpJob");

	JOBOBJECT_BASIC_LIMIT_INFORMATION linfo{};
	linfo.ActiveProcessLimit = 5;
	linfo.LimitFlags = JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
	SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &linfo, sizeof(linfo));

	HANDLE hCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	JOBOBJECT_ASSOCIATE_COMPLETION_PORT cp;
	cp.CompletionPort = hCP;
	cp.CompletionKey = 0;
	SetInformationJobObject(hJob, JobObjectAssociateCompletionPortInformation, 
		&cp, sizeof(cp));

	CreateThread(nullptr, 0, MonitorJob, hCP, 0, nullptr);

	WCHAR path[MAX_PATH];
	STARTUPINFO si{ sizeof(si) };
	PROCESS_INFORMATION pi;

	GetModuleFileName(nullptr, path, _countof(path));
	*wcsrchr(path, L'\\') = 0;
	wcscat_s(path, L"\\Runaway.exe");

	if (!CreateProcess(nullptr, path, nullptr, nullptr, FALSE, 0,
		nullptr, nullptr, &si, &pi))
		return 1;

	AssignProcessToJobObject(hJob, pi.hProcess);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	SetConsoleCtrlHandler([](DWORD) -> BOOL { 
		TerminateJobObject(hJob, 0); 
		ExitProcess(0);
		return TRUE; 
		}, TRUE);

	for (;;) {
		Sleep(1000);
		JOBOBJECT_BASIC_ACCOUNTING_INFORMATION info;
		QueryInformationJobObject(hJob, JobObjectBasicAccountingInformation,
			&info, sizeof(info), nullptr);
		printf("Processes: %u Total: %u\n",
			info.ActiveProcesses, info.TotalProcesses);
	}
	return 0;

}

