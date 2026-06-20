// LogonSessions.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <NTSecAPI.h>
#include <stdio.h>

#pragma comment(lib, "secur32")

int main() {
	ULONG count;
	LUID* pids;
	if (0 == LsaEnumerateLogonSessions(&count, &pids)) {
		for (ULONG i = 0; i < count; i++) {
			SECURITY_LOGON_SESSION_DATA* data;
			if (0 == LsaGetLogonSessionData(&pids[i], &data)) {
				printf("0x%X:%08X User: %wZ Session: %u Type: %u\n",
					data->LogonId.HighPart, data->LogonId.LowPart, 
					data->UserName, data->Session, data->LogonType);
				LsaFreeReturnBuffer(data);
			}
		}
		LsaFreeReturnBuffer(pids);
	}
	return 0;
}

