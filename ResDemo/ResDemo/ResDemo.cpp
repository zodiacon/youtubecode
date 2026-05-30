// ResDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include "resource.h"
#include <stdio.h>
#include <cassert>

int main() {
	WCHAR text[32];
	int count = LoadString(nullptr, IDS_HELLO, text, _countof(text));
	printf("%ws\n", text);

	HICON hIcon = LoadIcon(nullptr, MAKEINTRESOURCE(103));

	auto rs = FindResource(nullptr, MAKEINTRESOURCE(IDR_OBJEXP), L"BIN");
	assert(rs);

	HGLOBAL h = LoadResource(nullptr, rs);
	void* p = GlobalLock(h);
	assert(p);

	auto size = SizeofResource(nullptr, rs);

	auto hFile = CreateFile(L"d:\\temp\\test.exe", GENERIC_WRITE, 0,
		nullptr, OPEN_ALWAYS, 0, nullptr);
	DWORD written;
	WriteFile(hFile, p, size, &written, nullptr);
	CloseHandle(hFile);
	GlobalUnlock(h);

	return 0;
}

