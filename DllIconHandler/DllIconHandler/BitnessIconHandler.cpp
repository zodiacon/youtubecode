// BitnessIconHandler.cpp : Implementation of CBitnessIconHandler

#include "pch.h"
#include "BitnessIconHandler.h"
#include <DbgHelp.h>

#pragma comment(lib, "dbghelp")

// CBitnessIconHandler

HRESULT __stdcall CBitnessIconHandler::GetClassID(CLSID* pClassID) {
    return E_NOTIMPL;
}

HRESULT __stdcall CBitnessIconHandler::IsDirty(void) {
    return E_NOTIMPL;
}

HRESULT __stdcall CBitnessIconHandler::Load(LPCOLESTR pszFileName, DWORD dwMode) {
    auto hDll = LoadLibraryEx(pszFileName, nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if(!hDll)
        return S_OK;

    hDll = (HMODULE)((ULONG_PTR)hDll & ~1);
    auto header = ImageNtHeader(hDll);
    if (header) {
        auto machine = header->FileHeader.Machine;
        m_Bitness = header->Signature == IMAGE_NT_OPTIONAL_HDR64_MAGIC || machine == IMAGE_FILE_MACHINE_AMD64 
            || machine == IMAGE_FILE_MACHINE_ARM64 ? ModuleBitness::Bits64 : ModuleBitness::Bits32;
        ATLTRACE(L"Bitness of %s is %d\n", pszFileName, m_Bitness);
    }
    FreeLibrary(hDll);
    return S_OK;
}

HRESULT __stdcall CBitnessIconHandler::Save(LPCOLESTR pszFileName, BOOL fRemember) {
    return E_NOTIMPL;
}

HRESULT __stdcall CBitnessIconHandler::SaveCompleted(LPCOLESTR pszFileName) {
    return E_NOTIMPL;
}

HRESULT __stdcall CBitnessIconHandler::GetCurFile(LPOLESTR* ppszFileName) {
    return E_NOTIMPL;
}

HRESULT __stdcall CBitnessIconHandler::GetIconLocation(UINT uFlags, PWSTR pszIconFile, UINT cchMax, int* piIndex, UINT* pwFlags) {
    static WCHAR path[MAX_PATH];
    if (path[0] == 0) {
        GetModuleFileName(_pModule->GetModuleInstance(), path, _countof(path));
    }

    if (path[0] == 0)
        return S_FALSE;

    if (m_Bitness == ModuleBitness::Unknown)
        return S_FALSE;

    wcscpy_s(pszIconFile, cchMax, path);
    *piIndex = m_Bitness == ModuleBitness::Bits32 ? 0 : 1;
    *pwFlags = GIL_PERINSTANCE;
    ATLTRACE(L"Icon index for file %s (%d)\n", pszIconFile, *piIndex);

    return S_OK;
}

HRESULT __stdcall CBitnessIconHandler::Extract(PCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize) {
    return S_FALSE;
}
