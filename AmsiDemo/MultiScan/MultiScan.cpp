// MultiScan.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <amsi.h>
#include <stdio.h>
#include <atlcomcli.h>
#include <string>

struct SimpleStream : IAmsiStream {
	SimpleStream(PCWSTR path) : m_Path(path) {
	}

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override {
		if (riid == __uuidof(IAmsiStream) || riid == __uuidof(IUnknown)) {
			*ppvObject = static_cast<IAmsiStream*>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG __stdcall AddRef(void) override {
		return 2;
	}
	ULONG __stdcall Release(void) override {
		return 1;
	}
	HRESULT __stdcall GetAttribute(AMSI_ATTRIBUTE attribute, ULONG dataSize, unsigned char* data, ULONG* retData) override {
		switch (attribute) {
			case AMSI_ATTRIBUTE_SESSION:
				data[0] = 0;
				return S_OK;

			case AMSI_ATTRIBUTE_APP_NAME:
			{
				const WCHAR name[] = L"Sample";
				if (dataSize < sizeof(name)) {
					*retData = sizeof(name);
					return E_NOT_SUFFICIENT_BUFFER;
				}

				wcscpy_s((PWSTR)data, _countof(name), name);
				*retData = sizeof(name);
				return S_OK;
			}
			case AMSI_ATTRIBUTE_CONTENT_SIZE:
			{
				HANDLE hFile = CreateFile(m_Path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
					0, nullptr);
				if (hFile == INVALID_HANDLE_VALUE)
					return HRESULT_FROM_WIN32(GetLastError());
				LARGE_INTEGER size;
				GetFileSizeEx(hFile, &size);
				*(ULONGLONG*)data = size.QuadPart;
				*retData = sizeof(ULONGLONG);
				CloseHandle(hFile);
				return S_OK;
			}
		}
		return E_NOTIMPL;
	}

	HRESULT __stdcall Read(ULONGLONG position, ULONG size, unsigned char* buffer, ULONG* readSize) override {
		HANDLE hFile = CreateFile(m_Path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
			0, nullptr);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(GetLastError());

		SetFilePointerEx(hFile, { .QuadPart = (LONGLONG)position }, nullptr, SEEK_SET);
		auto ok = ReadFile(hFile, buffer, size, readSize, nullptr);
		CloseHandle(hFile);
		return ok ? S_OK : E_FAIL;
	}

private:
	std::wstring m_Path;
};


HRESULT ScanFile(PCWSTR path, GUID const& clsid, bool& isMalware) {
	CComPtr<IAntimalwareProvider> provider;
	auto hr = provider.CoCreateInstance(clsid);
	if (FAILED(hr))
		return hr;

	PWSTR display;
	if (SUCCEEDED(provider->DisplayName(&display))) {
		printf("Provider: %ws\n", display);
		CoTaskMemFree(display);
	}

	AMSI_RESULT result;
	SimpleStream ss(path);
	hr = provider->Scan(&ss, &result);
	if (FAILED(hr))
		return hr;

	isMalware = AmsiResultIsMalware(result);
	return S_OK;
}

int wmain(int argc, const wchar_t* argv[]) {
	if (argc < 2) {
		printf("Usage: AmsiDemo <file>\n");
		return 0;
	}

	CoInitialize(nullptr);

	HKEY hKey;
	auto status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\AMSI\\Providers",
		0, KEY_ENUMERATE_SUB_KEYS, &hKey);
	if (status != ERROR_SUCCESS)
		return 1;

	WCHAR keyName[128];
	DWORD keyLen;
	for (ULONG i = 0; ; i++) {
		keyLen = _countof(keyName);
		status = RegEnumKeyEx(hKey, i, keyName, &keyLen, nullptr, nullptr, nullptr, nullptr);
		if (status != ERROR_SUCCESS)
			break;

		CLSID clsid;
		CLSIDFromString(keyName, &clsid);
		bool isMalware;
		auto hr = ScanFile(argv[1], clsid, isMalware);
		if (FAILED(hr)) {
			printf("Error in ScanFile (0x%X)\n", hr);
			continue;
		}
		printf("Scan complete. Malware: %s\n", isMalware ? "Yes" : "No");
	}

	return 0;
}

