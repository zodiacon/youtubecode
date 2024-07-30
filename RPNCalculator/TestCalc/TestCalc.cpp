// TestCalc.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include "..\RPNCalcSvr\RPNCalcSvr.h"

int main() {
	CoInitialize(nullptr);

	IRPNCalculator* pCalc;
	auto hr = CoCreateInstance(__uuidof(RPNSimpleCalulator), nullptr, CLSCTX_ALL,
		__uuidof(IRPNCalculator), (void**)&pCalc);
	if (SUCCEEDED(hr)) {
		pCalc->Push(1);
		pCalc->Push(2);
		pCalc->Push(3);
		pCalc->Mul();
		pCalc->Add();
		int result;
		pCalc->Pop(&result);
		printf("Result: %d\n", result);
		pCalc->Release();
	}

	return 0;
}

