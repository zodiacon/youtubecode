#pragma once

#include <Unknwn.h>

struct __declspec(uuid("166978B1-C2DE-4BA6-93CA-D625660DC748")) RPNSimpleCalulator;

struct __declspec(uuid("525B283A-BE85-4515-904D-B289CF0BA0A6")) IRPNCalculator : IUnknown {
	virtual HRESULT __stdcall Push(int n) = 0;
	virtual HRESULT __stdcall Pop(int* n) = 0;
	virtual HRESULT __stdcall Add() = 0;
	virtual HRESULT __stdcall Sub() = 0;
	virtual HRESULT __stdcall Mul() = 0;
	virtual HRESULT __stdcall Div() = 0;
	virtual HRESULT __stdcall IsEmpty(bool* pEmpty) = 0;
};



