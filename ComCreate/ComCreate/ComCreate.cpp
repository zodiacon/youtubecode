// ComCreate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <ShlObj.h>
#include <string>

HRESULT MyCoGetClassObject(
	_In_ REFCLSID rclsid,
	_In_ DWORD dwClsContext,
	_In_opt_ LPVOID pvReserved,
	_In_ REFIID riid,
	_Outptr_ LPVOID  FAR* ppv) {
	HKEY hKey;
	WCHAR sclsid[64];
	StringFromGUID2(rclsid, sclsid, _countof(sclsid));
	std::wstring path(L"CLSID\\");
	path += sclsid;
	path += L"\\InprocServer32";
	auto error = RegOpenKeyEx(HKEY_CLASSES_ROOT, path.c_str(),
		0, KEY_READ, &hKey);
	if (error != ERROR_SUCCESS)
		return error;

	WCHAR dllPath[MAX_PATH];
	DWORD type = REG_EXPAND_SZ;
	DWORD size = sizeof(dllPath);
	error = RegQueryValueEx(hKey, L"", nullptr, &type, (PBYTE)dllPath, &size);
	if (error != ERROR_SUCCESS)
		return error;

	ExpandEnvironmentStrings(dllPath, dllPath, _countof(dllPath));

	auto hDll = CoLoadLibrary(dllPath, TRUE);
	if (!hDll)
		return HRESULT_FROM_WIN32(GetLastError());

	auto pDllGetClassObject = (decltype(DllGetClassObject)*)
		GetProcAddress(hDll, "DllGetClassObject");
	if (!pDllGetClassObject)
		return HRESULT_FROM_WIN32(GetLastError());

	return pDllGetClassObject(rclsid, riid, ppv);

}

HRESULT MyCoCreateInstance(
	_In_ REFCLSID rclsid,
	_In_opt_ LPUNKNOWN pUnkOuter,
	_In_ DWORD dwClsContext,
	_In_ REFIID riid,
	LPVOID  FAR* ppv) {

	IClassFactory* pCF;
	auto hr = MyCoGetClassObject(rclsid, dwClsContext, nullptr, 
		__uuidof(IClassFactory), (void**)&pCF);
	if (FAILED(hr))
		return hr;

	hr = pCF->CreateInstance(pUnkOuter, riid, ppv);
	pCF->Release();
	return hr;
}

int main() {
	CoInitialize(nullptr);

	CLSID clsid;
	auto hr = CLSIDFromProgID(L"shell.application", &clsid);
	if (FAILED(hr))
		return hr;

	IShellDispatch* pShell;
	IDispatch* pDisp;
	hr = MyCoCreateInstance(clsid, nullptr, CLSCTX_ALL, __uuidof(IDispatch),
		(void**)&pDisp);
	if (FAILED(hr))
		return hr;

	hr = pDisp->QueryInterface(__uuidof(IShellDispatch), (void**)&pShell);
	if (FAILED(hr))
		return hr;

	VARIANT param;
	param.vt = VT_BSTR;
	param.bstrVal = SysAllocString(L"c:\\windows\\system32");
	hr = pShell->Explore(param);

	pShell->Release();
	pDisp->Release();

	return 0;
}

