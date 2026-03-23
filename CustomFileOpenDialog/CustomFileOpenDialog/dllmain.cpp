// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Objbase.h>
#include <shobjidl_core.h>
#include <atlbase.h>
#include <string>

struct __declspec(uuid("{0B6A75CF-347A-4A8B-BDF8-8FF8D02177C6}")) MyFileOpenDialog : IFileOpenDialog {
	MyFileOpenDialog(IFileOpenDialog* dlg) : m_Dlg(dlg) {}

	HRESULT QueryInterface(const IID& riid, void** ppvObject) override {
		if (riid == __uuidof(IUnknown) || riid == __uuidof(IFileOpenDialog) || riid == __uuidof(IFileDialog)
			|| riid == __uuidof(IModalWindow)) {
			*ppvObject = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		return m_Dlg->QueryInterface(riid, ppvObject);
	}

	ULONG AddRef() override {
		return ++m_RefCount;
	}

	ULONG Release() override {
		auto count = --m_RefCount;
		if (count == 0)
			delete this;

		return count;
	}

	HRESULT Show(HWND hwndOwner) override {
		return m_Dlg->Show(hwndOwner);
	}

	HRESULT SetFileTypes(UINT cFileTypes, const COMDLG_FILTERSPEC* rgFilterSpec) override {
		return m_Dlg->SetFileTypes(cFileTypes, rgFilterSpec);
	}

	HRESULT SetFileTypeIndex(UINT iFileType) override {
		return m_Dlg->SetFileTypeIndex(iFileType);
	}

	HRESULT GetFileTypeIndex(UINT* piFileType) override {
		return m_Dlg->GetFileTypeIndex(piFileType);
	}

	HRESULT Advise(IFileDialogEvents* pfde, DWORD* pdwCookie) override {
		return m_Dlg->Advise(pfde, pdwCookie);
	}

	HRESULT Unadvise(DWORD dwCookie) override {
		return m_Dlg->Unadvise(dwCookie);
	}

	HRESULT SetOptions(FILEOPENDIALOGOPTIONS fos) override {
		return m_Dlg->SetOptions(fos);
	}

	HRESULT GetOptions(FILEOPENDIALOGOPTIONS* pfos) override {
		return m_Dlg->GetOptions(pfos);
	}

	HRESULT SetDefaultFolder(IShellItem* psi) override {
		return m_Dlg->SetDefaultFolder(psi);
	}

	HRESULT SetFolder(IShellItem* psi) override {
		return m_Dlg->SetFolder(psi);
	}

	HRESULT GetFolder(IShellItem** ppsi) override {
		return m_Dlg->GetFolder(ppsi);
	}

	HRESULT GetCurrentSelection(IShellItem** ppsi) override {
		return m_Dlg->GetCurrentSelection(ppsi);
	}

	HRESULT SetFileName(LPCWSTR pszName) override {
		return m_Dlg->SetFileName(pszName);
	}

	HRESULT GetFileName(LPWSTR* pszName) override {
		return m_Dlg->GetFileName(pszName);
	}

	HRESULT SetTitle(LPCWSTR pszTitle) override {
		return m_Dlg->SetTitle((pszTitle + std::wstring(L" This is my title!!")).c_str());
	}

	HRESULT SetOkButtonLabel(LPCWSTR pszText) override {
		return m_Dlg->SetOkButtonLabel(pszText);
	}

	HRESULT SetFileNameLabel(LPCWSTR pszLabel) override {
		return m_Dlg->SetFileNameLabel(pszLabel);
	}

	HRESULT GetResult(IShellItem** ppsi) override {
		return m_Dlg->GetResult(ppsi);
	}

	HRESULT AddPlace(IShellItem* psi, FDAP fdap) override {
		return m_Dlg->AddPlace(psi, fdap);
	}

	HRESULT SetDefaultExtension(LPCWSTR pszDefaultExtension) override {
		return m_Dlg->SetDefaultExtension(pszDefaultExtension);
	}

	HRESULT Close(HRESULT hr) override {
		return m_Dlg->Close(hr);
	}

	HRESULT SetClientGuid(const GUID& guid) override {
		return m_Dlg->SetClientGuid(guid);
	}

	HRESULT ClearClientData() override {
		return m_Dlg->ClearClientData();
	}

	HRESULT SetFilter(IShellItemFilter* pFilter) override {
		return m_Dlg->SetFilter(pFilter);
	}

	HRESULT GetResults(IShellItemArray** ppenum) override {
		return m_Dlg->GetResults(ppenum);
	}

	HRESULT GetSelectedItems(IShellItemArray** ppsai) override {
		return m_Dlg->GetSelectedItems(ppsai);
	}

	CComPtr<IFileOpenDialog> m_Dlg;
	ULONG m_RefCount{ 1 };
};

thread_local bool g_AllowDefault = false;

struct ActivationFilter : IActivationFilter {
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override {
		if (riid == __uuidof(IUnknown) || riid == __uuidof(IActivationFilter)) {
			*ppvObject = static_cast<IUnknown*>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	ULONG AddRef() override {
		return 1;
	}

	ULONG Release() override {
		return 1;
	}

	HRESULT HandleActivation(DWORD dwActivationType, const IID& rclsid, CLSID* pReplacementClsId) override {
		if (!g_AllowDefault && rclsid == CLSID_FileOpenDialog) {
			*pReplacementClsId = __uuidof(MyFileOpenDialog);
		}
		return S_OK;
	}
};

struct CustomFileOpenDialogFactory : IClassFactory {
	HRESULT QueryInterface(const IID& riid, void** ppvObject) override {
		if (riid == __uuidof(IUnknown) || riid == __uuidof(IClassFactory)) {
			*ppvObject = static_cast<IUnknown*>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	ULONG AddRef() override {
		return 1;
	}

	ULONG Release() override {
		return 1;
	}

	HRESULT CreateInstance(IUnknown* pUnkOuter, const IID& riid, void** ppvObject) override {
		CComPtr<IFileOpenDialog> dlg;
		g_AllowDefault = true;
		dlg.CoCreateInstance(__uuidof(FileOpenDialog));
		g_AllowDefault = false;
		auto customDlg = new MyFileOpenDialog(dlg);
		return customDlg->QueryInterface(riid, ppvObject);
	}

	HRESULT LockServer(BOOL fLock) override {
		return E_NOTIMPL;
	}
};

ActivationFilter g_Filter;
CustomFileOpenDialogFactory g_Factory;

extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppv) {
	if (clsid == __uuidof(MyFileOpenDialog)) {
		return g_Factory.QueryInterface(riid, ppv);
	}
	return CLASS_E_CLASSNOTAVAILABLE;
}

bool Init(HMODULE hModule) {
	CRegKey key;
	if (ERROR_SUCCESS != key.Create(HKEY_CURRENT_USER, L"Software\\Classes\\CLSID\\{0B6A75CF-347A-4A8B-BDF8-8FF8D02177C6}\\InProcServer32"))
		return false;

	WCHAR path[MAX_PATH];
	GetModuleFileName(hModule, path, _countof(path));
	key.SetStringValue(L"", path);

	if (FAILED(CoRegisterActivationFilter(&g_Filter)))
		return false;

	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			return Init(hModule);

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

