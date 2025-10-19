// AmsiDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <amsi.h>
#include <stdio.h>

#pragma comment(lib, "amsi")

int Error(DWORD code = GetLastError()) {
	printf("Error: %u\n", code);
	return 1;
}

int wmain(int argc, const wchar_t* argv[]) {
	if (argc < 2) {
		printf("Usage: AmsiDemo <file>\n");
		return 0;
	}

	auto hFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ,
		nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE)
		return Error();

	HAMSICONTEXT context;
	auto hr = AmsiInitialize(L"AmsiDemo", &context);
	if (FAILED(hr))
		return Error(hr);

	HAMSISESSION session;
	hr = AmsiOpenSession(context, &session);
	if (FAILED(hr))
		return Error(hr);

	auto hMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (!hMap)
		return Error();

	auto buffer = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	if (!buffer)
		return Error();

	AMSI_RESULT result;
	hr = AmsiScanBuffer(context, buffer, GetFileSize(hFile, nullptr),
		argv[1], session, &result);
	if (FAILED(hr))
		return Error(hr);

	printf("Malware detected: %s\n", AmsiResultIsMalware(result) ? "Yes" : "No");

	return 0;
}
