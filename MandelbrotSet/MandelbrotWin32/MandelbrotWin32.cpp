// MandelbrotWin32.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "MandelbrotWin32.h"
#include <complex>
#include <windowsx.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

using Complex = std::complex<double>;

int MandelbrotColor(Complex const& c) {
	int count = 255;
	Complex z = 0;

	while (count > 0 && std::norm(z) <= 4) {
		z = z * z + c;
		count--;
	}
	return count;
}

void DrawMandelbrot(HDC hdc, int width, int height, Complex const& from, Complex const& to) {
	auto dx = (to - from).real() / width;
	auto dy = (to - from).imag() / height;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			Complex c(dx * x + from.real(), dy * y + from.imag());
			auto count = MandelbrotColor(c);
			auto color = RGB(count, count, count);
			SetPixelV(hdc, x, y, color);
		}
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MANDELBROTWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROTWIN32));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROTWIN32));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MANDELBROTWIN32);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance; // Store instance handle in our global variable

	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HBITMAP hBitmap;
	static int bmpWidth = 800, bmpHeight = 600;
	static Complex from(-1.7, -1.2), to(0.8, 1.2);
	static bool mouseDown = false;
	static RECT selectionRect;

	switch (message) {
		case WM_CREATE:
		{
			HDC hdc = GetDC(hWnd);
			hBitmap = CreateCompatibleBitmap(hdc, bmpWidth, bmpHeight);
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			DrawMandelbrot(hdcMem, bmpWidth, bmpHeight, from, to);
			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
		}

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId) {
				case ID_VIEW_RESET:
				{
					from = Complex(-1.7, -1.2);
					to = Complex(0.8, 1.2);
					DeleteObject(hBitmap);
					HDC hdc = GetDC(hWnd);
					hBitmap = CreateCompatibleBitmap(hdc, bmpWidth, bmpHeight);
					HDC hdcMem = CreateCompatibleDC(hdc);
					SelectObject(hdcMem, hBitmap);
					DrawMandelbrot(hdcMem, bmpWidth, bmpHeight, from, to);
					DeleteDC(hdcMem);
					ReleaseDC(hWnd, hdc);
					InvalidateRect(hWnd, nullptr, FALSE);
					break;
				}
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_LBUTTONDOWN:
			mouseDown = true;
			selectionRect.left = GET_X_LPARAM(lParam);
			selectionRect.top = GET_Y_LPARAM(lParam);
			SetCapture(hWnd);
			break;

		case WM_ERASEBKGND:
			return 1;

		case WM_LBUTTONUP:
		{
			mouseDown = false;
			ReleaseCapture();

			RECT rc;
			GetClientRect(hWnd, &rc);
			auto size = to - from;
			from = Complex(from.real() + min(selectionRect.left, selectionRect.right) * size.real() / rc.right,
				from.imag() + min(selectionRect.top, selectionRect.bottom) * size.imag() / rc.bottom);
			to = Complex(from.real() + size.real() * abs(selectionRect.right - selectionRect.left) / rc.right,
				from.imag() + size.imag() * abs(selectionRect.bottom - selectionRect.top) / rc.bottom);
			DeleteObject(hBitmap);
			bmpWidth = rc.right; bmpHeight = rc.bottom;
			HDC hdc = GetDC(hWnd);
			hBitmap = CreateCompatibleBitmap(hdc, bmpWidth, bmpHeight);
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			DrawMandelbrot(hdcMem, bmpWidth, bmpHeight, from, to);
			DeleteDC(hdcMem);
			ReleaseDC(hWnd, hdc);
			InvalidateRect(hWnd, nullptr, FALSE);
			break;
		}

		case WM_MOUSEMOVE:
			if (mouseDown) {
				selectionRect.right = GET_X_LPARAM(lParam);
				selectionRect.bottom = GET_Y_LPARAM(lParam);
				InvalidateRect(hWnd, nullptr, FALSE);
			}
			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			
			HDC hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			RECT rc;
			GetClientRect(hWnd, &rc);
			StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bmpWidth, bmpHeight, SRCCOPY);
			DeleteDC(hdcMem);

			if (mouseDown) {
				SelectObject(hdc, GetStockObject(DC_PEN));
				SetDCPenColor(hdc, RGB(255, 0, 0));
				SelectObject(hdc, GetStockObject(NULL_BRUSH));
				Rectangle(hdc, selectionRect.left, selectionRect.top, selectionRect.right, selectionRect.bottom);
			}

			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}
