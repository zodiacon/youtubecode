#include "pch.h"
#include "RPNCalcCF.h"
#include "RPNCalc.h"

HRESULT __stdcall RPNCalcCF::QueryInterface(REFIID riid, void** ppvObject) {
    if (riid == __uuidof(IUnknown) || riid == __uuidof(IClassFactory)) {
        *ppvObject = static_cast<IClassFactory*>(this);
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG __stdcall RPNCalcCF::AddRef(void) {
    return 3;
}

ULONG __stdcall RPNCalcCF::Release(void) {
    return 2;
}

HRESULT __stdcall RPNCalcCF::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) {
    if (pUnkOuter)
        return E_INVALIDARG;

    auto calc = new RPNCalc;    // RefCount=1
    auto hr = calc->QueryInterface(riid, ppvObject);    // RefCount=2 if successful
    calc->Release();    // if successful, RefCount=1. Otherwise, object will be destroyed!
    return hr;
}

HRESULT __stdcall RPNCalcCF::LockServer(BOOL fLock) {
    return E_NOTIMPL;
}
