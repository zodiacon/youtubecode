// RPNCalcSvr.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "RPNCalcSvr.h"
#include "RPNCalcCF.h"

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppvObject) {
	if (clsid == __uuidof(RPNSimpleCalulator)) {
		static RPNCalcCF cf;
		return cf.QueryInterface(riid, ppvObject);
	}
	return CLASS_E_CLASSNOTAVAILABLE;
}

extern HINSTANCE g_hInstDll;

extern "C" HRESULT __stdcall DllRegisterServer() {
	HKEY hKey;
	auto error = RegCreateKeyEx(HKEY_CURRENT_USER, L"Software\\Classes\\CLSID\\{166978B1-C2DE-4BA6-93CA-D625660DC748}\\InProcServer32",
		0, nullptr, 0, KEY_WRITE, nullptr, &hKey, nullptr);
	if (error != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(error);

	WCHAR path[MAX_PATH];
	GetModuleFileName(g_hInstDll, path, _countof(path));
	error = RegSetValueExW(hKey, L"", 0, REG_SZ, (const PBYTE)path, DWORD(wcslen(path) + 1) * sizeof(WCHAR));
	if (error != ERROR_SUCCESS)
		return HRESULT_FROM_WIN32(error);

	RegCloseKey(hKey);
	return S_OK;
}
