// CaptureWindow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <wincodec.h>
#include <atlcomcli.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi")

HBITMAP CaptureWindow(HWND hWnd) {
	WINDOWINFO wi;
	GetWindowInfo(hWnd, &wi);
	RECT rc = wi.rcClient;
	//GetClientRect(hWnd, &rc);
	HDC hdc = GetDC(hWnd);
	HBITMAP hBmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBmp);
	BitBlt(hdcMem, 0, 0, rc.right, rc.bottom, hdc, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
	DeleteDC(hdcMem);

	return hBmp;
}

bool SaveBitmap(HBITMAP hBmp, PCWSTR path) {
	CComPtr<IWICImagingFactory> factory;
	auto hr = factory.CoCreateInstance(CLSID_WICImagingFactory);
	if (FAILED(hr))
		return false;

	CComPtr<IWICBitmap> bitmap;
	hr = factory->CreateBitmapFromHBITMAP(hBmp, nullptr, WICBitmapIgnoreAlpha, &bitmap);
	ATLASSERT(SUCCEEDED(hr));

	CComPtr<IWICBitmapEncoder> encoder;
	hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
	ATLASSERT(SUCCEEDED(hr));

	CComPtr<IStream> stm;
	hr = SHCreateStreamOnFileEx(path, STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
		0, TRUE, nullptr, &stm);
	ATLASSERT(SUCCEEDED(hr));

	hr = encoder->Initialize(stm, WICBitmapEncoderNoCache);
	ATLASSERT(SUCCEEDED(hr));

	CComPtr<IWICBitmapFrameEncode> frame;
	hr = encoder->CreateNewFrame(&frame, nullptr);
	ATLASSERT(SUCCEEDED(hr));

	frame->Initialize(nullptr);
	hr = frame->WriteSource(bitmap, nullptr);
	ATLASSERT(SUCCEEDED(hr));

	frame->Commit();
	encoder->Commit();

	return true;
}

int main() {
	CoInitialize(nullptr);

	auto hWnd = GetDesktopWindow();
	auto h = CaptureWindow(hWnd);
	SaveBitmap(h, L"d:\\temp\\images\\pic.png");
	return 0;
}
