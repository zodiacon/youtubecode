#include <Windows.h>
#include <detours.h>
#include <atlbase.h>

LRESULT CALLBACK MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			
			RECT rc;
			GetClientRect(hWnd, &rc);
			Ellipse(hdc, 0, 0, rc.right, rc.bottom);

			EndPaint(hWnd, &ps);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

decltype(::Ellipse)* OrgEllipse = ::Ellipse;
decltype(::HeapAlloc)* OrgHeapAlloc = ::HeapAlloc;

PVOID WINAPI HookHeapAlloc(HANDLE hHeap, DWORD flags, SIZE_T size) {
	return OrgHeapAlloc(hHeap, flags, size);
}

BOOL WINAPI HookEllipse(HDC hdc, int x1, int y1, int x2, int y2) {
	ATLTRACE(L"Ellipse called (%d,%d)-(%d,%d)\n",
		x1, y1, x2, y2);
	x1 += 30; y1 += 30;
	x2 -= 30; y2 -= 30;
	HPEN hPen = CreatePen(PS_SOLID, 6, RGB(255, 0, 0));
	auto hOldPen = SelectObject(hdc, hPen);
	auto rv = OrgEllipse(hdc, x1, y1, x2, y2);
	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
	return rv;
}

void InstallHooks() {
	DetourTransactionBegin();

	DetourAttach((PVOID*)&OrgEllipse, HookEllipse);
	DetourAttach((PVOID*)&OrgHeapAlloc, HookHeapAlloc);

	DetourTransactionCommit();
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR cmdLine, int cmdShow) {
	InstallHooks();

	WNDCLASS wc{};
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.lpszClassName = L"MyWinAppWndClass";
	wc.lpfnWndProc = MyWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInstance;
	auto atom = RegisterClass(&wc);

	HWND hWnd = CreateWindow(L"MyWinAppWndClass", L"My WinApp Application", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
		return 1;

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
