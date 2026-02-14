// MyApplet.cpp : Defines the exported functions for the DLL.
//

#include "framework.h"
#include "MyApplet.h"
#include <cpl.h>
#include "Resource.h"

// This is an example of an exported function.
extern "C" MYAPPLET_API LONG __stdcall CPlApplet(HWND hwndCpl, UINT msg, LPARAM lParam1, LPARAM lParam2) {
	switch (msg) {
		case CPL_INIT:
			return TRUE;

		case CPL_GETCOUNT:
			return 1;

		case CPL_INQUIRE:
		{
			auto info = (CPLINFO*)lParam2;
			info->idName = IDS_NAME;
			info->idInfo = IDS_INFO;
			info->idIcon = IDI_MYAPPLET;
			break;
		}

		case CPL_DBLCLK:
			MessageBox(hwndCpl, L"My great applet", L"My Applet", MB_ICONINFORMATION);
			break;
	}
	return 0;
}

