// JobPlay.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>

int main(int argc, const char* argv[]) {
	if (argc < 2)
		return 1;

	HANDLE hJob = CreateJobObject(nullptr, L"JobPlay");
	if (!hJob)
		return 1;

	int pid = strtol(argv[1], nullptr, 0);

	HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE, pid);
	if (!hProcess)
		return 1;

	if (!AssignProcessToJobObject(hJob, hProcess))
		return 1;

	JOBOBJECT_BASIC_LIMIT_INFORMATION info;
	info.ActiveProcessLimit = 2;
	info.LimitFlags = JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
	if (!SetInformationJobObject(hJob, JobObjectBasicLimitInformation, &info, sizeof(info)))
		return 1;

	JOBOBJECT_CPU_RATE_CONTROL_INFORMATION info2;
	info2.CpuRate = 1000;
	info2.ControlFlags = JOB_OBJECT_CPU_RATE_CONTROL_ENABLE |
		JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP;
	if (!SetInformationJobObject(hJob, JobObjectCpuRateControlInformation, &info2, sizeof(info2)))
		return 1;

	printf("Success!\n");
	Sleep(INFINITE);

	return 0;
}
