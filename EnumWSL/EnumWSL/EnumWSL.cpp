// EnumWSL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <atlcomcli.h>
#include "WslService.h"
#include <cassert>

int main() {
	CoInitialize(nullptr);

	CoInitializeSecurity(nullptr, -1, nullptr, nullptr, RPC_C_AUTHN_LEVEL_NONE,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE, nullptr);

	CComPtr<ILxssUserSession> session;
	auto hr = session.CoCreateInstance(__uuidof(LxssUserSession));
	assert(SUCCEEDED(hr));

	ULONG count;
	LXSS_ENUMERATE_INFO* info;
	LXSS_ERROR_INFO einfo{};
	hr = session->EnumerateDistributions(&count, &info, &einfo);
	if (SUCCEEDED(hr)) {
		for (ULONG i = 0; i < count; i++) {
			printf("%ws (Version: %d) State: %d\n",
				info[i].DistroName, info[i].Version, info[i].State);
		}
		CoTaskMemFree(info);
		if (einfo.Message)
			CoTaskMemFree(einfo.Message);
	}

	return 0;
}
