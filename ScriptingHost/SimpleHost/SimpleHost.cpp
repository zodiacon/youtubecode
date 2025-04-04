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

	engine->AddNamedItem(L"Shell", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_GLOBALMEMBERS);

	CComQIPtr<IActiveScriptParse> parse(engine);
	ATLASSERT(parse);

	CComVariant result;
	EXCEPINFO exceptInfo;
	hr = parse->ParseScriptText(LR"(
shellexecute "mspaint"
explore "c:\windows"
msgbox "hello"
	)", nullptr, nullptr, nullptr, 0, 1, 0, &result, &exceptInfo);

	return 0;

}
