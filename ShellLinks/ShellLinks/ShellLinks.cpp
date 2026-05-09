// ShellLinks.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <ShlObj.h>
#include <atlcomCli.h>

int main() {
	CoInitialize(nullptr);

	CComPtr<IShellLink> link;
	auto hr = link.CoCreateInstance(__uuidof(ShellLink));
	CComQIPtr<IPersistFile> persist(link);
	hr = persist->Load(LR"(C:\Users\Public\Desktop\VLC media player.lnk)", STGM_READ);

	WCHAR path[MAX_PATH];
	hr = link->GetPath(path, _countof(path), nullptr, 0);
	
	return 0;
}

