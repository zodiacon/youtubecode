#pragma once

#include <Windows.h>
#include <ActivScp.h>
#include <atlcomcli.h>

class ScriptHost : 
	public IActiveScriptSite,
	public IActiveScriptSiteWindow {
public:
	// Inherited via IActiveScriptSite
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;

	HRESULT __stdcall GetLCID(LCID* plcid) override;
	HRESULT __stdcall GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown** ppiunkItem, ITypeInfo** ppti) override;
	HRESULT __stdcall GetDocVersionString(BSTR* pbstrVersion) override;
	HRESULT __stdcall OnScriptTerminate(const VARIANT* pvarResult, const EXCEPINFO* pexcepinfo) override;
	HRESULT __stdcall OnStateChange(SCRIPTSTATE ssScriptState) override;
	HRESULT __stdcall OnScriptError(IActiveScriptError* pscripterror) override;
	HRESULT __stdcall OnEnterScript(void) override;
	HRESULT __stdcall OnLeaveScript(void) override;

	// Inherited via IActiveScriptSiteWindow
	HRESULT __stdcall GetWindow(HWND* phwnd) override;
	HRESULT __stdcall EnableModeless(BOOL fEnable) override;

private:
	CComPtr<IDispatch> m_Shell;
};

