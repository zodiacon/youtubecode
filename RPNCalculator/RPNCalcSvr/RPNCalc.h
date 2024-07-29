#pragma once

#include "RPNCalcSvr.h"
#include <stack>

class RPNCalc : IRPNCalculator {
public:
	// Inherited via IRPNCalculator
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;
	ULONG __stdcall AddRef(void) override;
	ULONG __stdcall Release(void) override;
	HRESULT __stdcall Push(int n) override;
	HRESULT __stdcall Pop(int* n) override;
	HRESULT __stdcall Add() override;
	HRESULT __stdcall Sub() override;
	HRESULT __stdcall Mul() override;
	HRESULT __stdcall Div() override;
	HRESULT __stdcall IsEmpty(bool* pEmpty) override;

private:
	std::stack<int> m_Stack;
	int m_RefCount{ 1 };
};

