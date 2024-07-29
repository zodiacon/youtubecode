// RPNCalcSvr.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "RPNCalcSvr.h"
#include "RPNCalcCF.h"

HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID riid, void** ppvObject) {
	if (clsid == __uuidof(RPNSimpleCalulator)) {
		static RPNCalcCF cf;
		return cf.QueryInterface(riid, ppvObject);
	}
	return CLASS_E_CLASSNOTAVAILABLE;
}
