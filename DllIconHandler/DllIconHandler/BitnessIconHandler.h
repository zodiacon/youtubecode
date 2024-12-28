// BitnessIconHandler.h : Declaration of the CBitnessIconHandler

#pragma once
#include "resource.h"       // main symbols
#include "DllIconHandler_i.h"
#include <ShlObj_core.h>

using namespace ATL;


// CBitnessIconHandler

class ATL_NO_VTABLE CBitnessIconHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IExtractIcon,
	public IPersistFile,
	public CComCoClass<CBitnessIconHandler, &CLSID_BitnessIconHandler> {
public:
	CBitnessIconHandler() {
	}

	DECLARE_REGISTRY_RESOURCEID(106)

	BEGIN_COM_MAP(CBitnessIconHandler)
		COM_INTERFACE_ENTRY(IPersistFile)
		COM_INTERFACE_ENTRY(IExtractIcon)
	END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct() {
		return S_OK;
	}

	void FinalRelease() {
	}

private:
	enum class ModuleBitness {
		Unknown,
		Bits32,
		Bits64
	};

	// Inherited via IPersistFile
	HRESULT __stdcall GetClassID(CLSID* pClassID) override;
	HRESULT __stdcall IsDirty(void) override;
	HRESULT __stdcall Load(LPCOLESTR pszFileName, DWORD dwMode) override;
	HRESULT __stdcall Save(LPCOLESTR pszFileName, BOOL fRemember) override;
	HRESULT __stdcall SaveCompleted(LPCOLESTR pszFileName) override;
	HRESULT __stdcall GetCurFile(LPOLESTR* ppszFileName) override;

	// Inherited via IExtractIconW
	HRESULT __stdcall GetIconLocation(UINT uFlags, PWSTR pszIconFile, UINT cchMax, int* piIndex, UINT* pwFlags) override;
	HRESULT __stdcall Extract(PCWSTR pszFile, UINT nIconIndex, HICON* phiconLarge, HICON* phiconSmall, UINT nIconSize) override;

	ModuleBitness m_Bitness{ ModuleBitness::Unknown };
};

OBJECT_ENTRY_AUTO(__uuidof(BitnessIconHandler), CBitnessIconHandler)
