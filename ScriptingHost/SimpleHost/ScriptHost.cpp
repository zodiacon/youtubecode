#include "ScriptHost.h"
#include <stdio.h>

HRESULT __stdcall ScriptHost::QueryInterface(REFIID riid, void** ppvObject) {
    if (riid == __uuidof(IUnknown) || riid == __uuidof(IActiveScriptSite)) {
        *ppvObject = static_cast<IActiveScriptSite*>(this);
        return S_OK;
    }
    if (riid == __uuidof(IActiveScriptSiteWindow)) {
        *ppvObject = static_cast<IActiveScriptSiteWindow*>(this);
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG __stdcall ScriptHost::AddRef(void) {
    return 2;
}

ULONG __stdcall ScriptHost::Release(void) {
    return 1;
}

HRESULT __stdcall ScriptHost::GetLCID(LCID* plcid) {
    return E_NOTIMPL;
}

HRESULT __stdcall ScriptHost::GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown** ppiunkItem, ITypeInfo** ppti) {
    if (_wcsicmp(pstrName, L"SHell") == 0) {
        if (m_Shell == nullptr)
            m_Shell.CoCreateInstance(L"shell.Application");
        return m_Shell.QueryInterface(ppiunkItem);
    }
    return E_INVALIDARG;
}

HRESULT __stdcall ScriptHost::GetDocVersionString(BSTR* pbstrVersion) {
    return E_NOTIMPL;
}

HRESULT __stdcall ScriptHost::OnScriptTerminate(const VARIANT* pvarResult, const EXCEPINFO* pexcepinfo) {
    return E_NOTIMPL;
}

HRESULT __stdcall ScriptHost::OnStateChange(SCRIPTSTATE ssScriptState) {
    return E_NOTIMPL;
}

HRESULT __stdcall ScriptHost::OnScriptError(IActiveScriptError* p) {
    EXCEPINFO info;
    p->GetExceptionInfo(&info);
    ULONG line;
    LONG col;
    p->GetSourcePosition(nullptr, &line, &col);
    printf("Error: %ws (Line: %u Col: %d)\n",
        info.bstrDescription, line, col);

    return S_OK;
}

HRESULT __stdcall ScriptHost::OnEnterScript(void) {
    return E_NOTIMPL;
}

HRESULT __stdcall ScriptHost::OnLeaveScript(void) {
    return E_NOTIMPL;
}

HRESULT __stdcall ScriptHost::GetWindow(HWND* phwnd) {
    *phwnd = GetDesktopWindow();

    return S_OK;
}

HRESULT __stdcall ScriptHost::EnableModeless(BOOL fEnable) {
    return S_OK;
}
