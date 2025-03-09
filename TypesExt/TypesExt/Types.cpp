#include <Windows.h>
#define KDEXT_64BIT
#include <DbgEng.h>
#include <WDBGEXTS.H>
#include <atlcomcli.h>

#pragma comment(lib, "dbgeng")

WINDBG_EXTENSION_APIS64 ExtensionApis{ sizeof(ExtensionApis) };

HRESULT __stdcall DebugExtensionInitialize(PULONG version, PULONG flags) {
	CComPtr<IDebugClient> client;
	auto hr = DebugCreate(__uuidof(IDebugClient), (void**)&client);
	if (FAILED(hr))
		return hr;

	CComQIPtr<IDebugControl> control(client);
	hr = control->GetWindbgExtensionApis64(&ExtensionApis);
	if (FAILED(hr))
		return hr;

	*version = DEBUG_EXTENSION_VERSION(1, 0);
	*flags = 0;

	return S_OK;
}

STDAPI help(IDebugClient* client, PCSTR args) {
	dprintf("ObjectTypes help\n");
	dprintf("  help - show this help\n");
	dprintf("  objecttypes [name] - display object types filtered by 'name'\n");

	return S_OK;
}

STDAPI objecttypes(IDebugClient* client, PCSTR args) {
	auto types = GetExpression("nt!ObpObjectTypes");
	if (types == 0) {
		dprintf("Failed to locate nt!ObpObjectTypes\n");
		return E_UNEXPECTED;
	}

	int count = 0;
	ULONG nameOffset;
	GetFieldOffset("nt!_OBJECT_TYPE", "Name", &nameOffset);

	while (types) {
		ULONG64 type;
		if (!ReadPointer(types, &type))
			break;

		if (type == 0)
			break;

		ULONG totalObjects, totalHandles;
		ULONG peakObjects, peakHandles;
		UCHAR index;
		GetFieldValue(type, "nt!_OBJECT_TYPE", "Index", index);
		GetFieldValue(type, "nt!_OBJECT_TYPE", "TotalNumberOfObjects", totalObjects);
		GetFieldValue(type, "nt!_OBJECT_TYPE", "TotalNumberOfHandles", totalHandles);
		GetFieldValue(type, "nt!_OBJECT_TYPE", "HighWaterNumberOfObjects", peakObjects);
		GetFieldValue(type, "nt!_OBJECT_TYPE", "HighWaterNumberOfHandles", peakHandles);

		CComQIPtr<IDebugControl> control(client);

		control->ControlledOutput(DEBUG_OUTCTL_DML, DEBUG_OUTPUT_NORMAL,
			"%-33msu <link cmd=\"dt nt!_OBJECT_TYPE %p\">0x%p</link> %3d %8u %8u %8u %8u\n",
			type + nameOffset, type, type, index, totalObjects, totalHandles, peakObjects, peakHandles);

		types += sizeof(void*);
		count++;
	}
	dprintf("Total objects: %d\n", count);

	return S_OK;
}

