// ComCreateWithMoniker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <atlcomcli.h>
#include <ShlObj.h>

int main() {
	CoInitialize(nullptr);

	CComPtr<IShellWindows> shell, shell2;
	//shell.CoCreateInstance(__uuidof(ShellWindows));

	CComPtr<IClassFactory> cf;
	//CoGetObject(L"clsid:9BA05972-F6A8-11CF-A442-00A0C90A8F39",
	//	nullptr, IID_PPV_ARGS(&cf));

	CComPtr<IBindCtx> bindCtx;
	CreateBindCtx(0, &bindCtx);
	ULONG eaten;
	CComPtr<IMoniker> moniker;
	MkParseDisplayName(bindCtx, L"clsid:9BA05972-F6A8-11CF-A442-00A0C90A8F39", 
		&eaten, &moniker);
	moniker->BindToObject(bindCtx, nullptr, IID_PPV_ARGS(&cf));

	cf->CreateInstance(nullptr, IID_PPV_ARGS(&shell));
	cf->CreateInstance(nullptr, IID_PPV_ARGS(&shell2));

	return 0;
}

