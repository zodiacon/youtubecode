// SimpleHost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <ActivScp.h>
#include <atlcomcli.h>
#include "ScriptHost.h"

int main() {
	CoInitialize(nullptr);

	CComPtr<IActiveScript> engine;
	auto hr = engine.CoCreateInstance(L"VBScript");
	if (FAILED(hr))
		return hr;

	ScriptHost host;
	hr = engine->SetScriptSite(&host);

	CComQIPtr<IActiveScriptParse> parse(engine);
	ATLASSERT(parse);

	CComVariant result;
	EXCEPINFO exceptInfo;
	hr = parse->ParseScriptText(LR"(
set shell = CreateObject("shell.application")
msgbox "Opening System32", vbInformation, "VBScript Example"
shell.ExploreX "c:\windows\System32"
	)", nullptr, nullptr, nullptr, 0, 1, 0, &result, &exceptInfo);

	return 0;

}
