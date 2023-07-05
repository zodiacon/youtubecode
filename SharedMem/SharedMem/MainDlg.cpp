// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	m_hSharedMem = CreateFileMapping(INVALID_HANDLE_VALUE,
		nullptr, PAGE_READWRITE, 0, 1 << 20, L"MySharedMemory");
	ATLASSERT(m_hSharedMem);

	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// TODO: Add validation code 
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnWrite(WORD, WORD wID, HWND, BOOL&) {
	void* p = MapViewOfFile(m_hSharedMem, FILE_MAP_WRITE, 0, 0, 64 << 10);
	ATLASSERT(p);

	WCHAR text[256];
	GetDlgItemText(IDC_TEXT, text, _countof(text));
	wcscpy_s((PWSTR)p, _countof(text), text);
	UnmapViewOfFile(p);

	return 0;
}

LRESULT CMainDlg::OnRead(WORD, WORD wID, HWND, BOOL&) {
	void* p = MapViewOfFile(m_hSharedMem, FILE_MAP_READ, 0, 0, 64 << 10);
	ATLASSERT(p);

	SetDlgItemText(IDC_TEXT, (PCWSTR)p);

	UnmapViewOfFile(p);

	return 0;
}
