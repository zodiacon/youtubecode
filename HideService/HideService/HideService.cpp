// HideService.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <sddl.h>

DWORD HideService(PCWSTR name) {
	auto hScm = OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
	if (!hScm)
		return GetLastError();

	auto hService = OpenService(hScm, name, SERVICE_ALL_ACCESS);
	if (!hService)
		return GetLastError();

	BYTE sd[1024];
	DWORD size;
	if (!QueryServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, sd,
		sizeof(sd), &size))
		return GetLastError();

	PWSTR sddl;
	if (ConvertSecurityDescriptorToStringSecurityDescriptor(sd, SDDL_REVISION,
		DACL_SECURITY_INFORMATION, &sddl, nullptr)) {
		printf("SDDL: %ws\n", sddl);
		LocalFree(sddl);
	}

	BYTE adminSid[SECURITY_MAX_SID_SIZE];
	DWORD adminSidSize = sizeof(adminSid);
	if (!CreateWellKnownSid(WinBuiltinAdministratorsSid, nullptr, adminSid, &adminSidSize))
		return GetLastError();

	BOOL daclPresent, daclDefaulted;
	PACL dacl;
	if (!GetSecurityDescriptorDacl(sd, &daclPresent, &dacl, &daclDefaulted))
		return GetLastError();

	//if (!AddAccessDeniedAce(dacl, ACL_REVISION, GENERIC_READ, adminSid))
	//	return GetLastError();

	//SetServiceObjectSecurity(

	return ERROR_SUCCESS;
}

int wmain(int argc, const wchar_t* argv[]) {
	if (argc < 2) {
		printf("Usage: HideService <servicename> [show]\n");
		return 0;
	}
	auto err = HideService(argv[1]);
	if (err)
		printf("Error: %u\n", err);
	else
		printf("Success!\n");
	return err;
}

