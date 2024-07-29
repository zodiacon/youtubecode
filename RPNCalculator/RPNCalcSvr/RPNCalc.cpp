#include "pch.h"
#include "RPNCalc.h"

HRESULT __stdcall RPNCalc::QueryInterface(REFIID riid, void** ppvObject) {
    if (riid == __uuidof(IUnknown) || riid == __uuidof(IRPNCalculator)) {
        *ppvObject = static_cast<IUnknown*>(this);
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

ULONG __stdcall RPNCalc::AddRef(void) {
    return ++m_RefCount;
}

ULONG __stdcall RPNCalc::Release(void) {
    int count = --m_RefCount;
    if (count == 0)
        delete this;
    return count;
}

HRESULT __stdcall RPNCalc::Push(int n) {
    m_Stack.push(n);
    return S_OK;
}

HRESULT __stdcall RPNCalc::Pop(int* n) {
    if (m_Stack.empty())
        return E_UNEXPECTED;

    *n = m_Stack.top();
    m_Stack.pop();
    return S_OK;
}

HRESULT __stdcall RPNCalc::Add() {
    if (m_Stack.size() < 2)
        return E_UNEXPECTED;
    int x, y;
    Pop(&x);
    Pop(&y);

    return Push(x + y);
}

HRESULT __stdcall RPNCalc::Sub() {
    if (m_Stack.size() < 2)
        return E_UNEXPECTED;
    int x, y;
    Pop(&x);
    Pop(&y);

    return Push(y - x);
}

HRESULT __stdcall RPNCalc::Mul() {
    if (m_Stack.size() < 2)
        return E_UNEXPECTED;
    int x, y;
    Pop(&x);
    Pop(&y);

    return Push(x * y);
}

HRESULT __stdcall RPNCalc::Div() {
    if (m_Stack.size() < 2)
        return E_UNEXPECTED;
    int x, y;
    Pop(&x);
    Pop(&y);
    if (x == 0)
        return E_UNEXPECTED;

    return Push(y / x);
}

HRESULT __stdcall RPNCalc::IsEmpty(bool* pEmpty) {
    *pEmpty = m_Stack.empty();
    return S_OK;
}
