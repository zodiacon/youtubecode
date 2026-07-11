// AppExecAlias.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <string>

int main() {
	WCHAR path[MAX_PATH];
	GetEnvironmentVariable(L"LocalAppData", path, _countof(path));
	wcscat_s(path, L"\\Microsoft\\WindowsApps\\");
	std::wstring dir(path);

	WIN32_FIND_DATA fd;
	auto hFind = FindFirstFile((dir + L"*.exe").c_str(), &fd);
	if (hFind == INVALID_HANDLE_VALUE)
		return 1;

	struct ReparseData {
		ULONG Tag;
		USHORT Size;
		USHORT Reserved;
		ULONG Version;
		WCHAR Text[0x800];
	};

	DWORD ret;
	do {
		printf("%ws\n", fd.cFileName);
		auto hFile = CreateFile((dir + fd.cFileName).c_str(), GENERIC_READ, FILE_SHARE_READ,
			nullptr, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
		if (hFile != INVALID_HANDLE_VALUE) {
			ReparseData data{};
			if (DeviceIoControl(hFile, FSCTL_GET_REPARSE_POINT, nullptr, 0,
				&data, sizeof(data), &ret, nullptr)) {
				printf("\tTag: 0x%X Size: %u\n", data.Tag, data.Size);
				auto p = data.Text;
				while (*p) {
					printf("\t%ws\n", p);
					p += wcslen(p) + 1;
				}
			}
			CloseHandle(hFile);
		}
	} while (FindNextFile(hFind, &fd));

	FindClose(hFind);

	return 0;
}
