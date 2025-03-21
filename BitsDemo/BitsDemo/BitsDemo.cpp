// BitsDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <stdio.h>
#include <Bits.h>
#include <atlcomcli.h>

int Error(HRESULT hr) {
	printf("Error: 0x%X\n", hr);
	return hr;
}

HANDLE g_hDone;

struct NotifyCallback : IBackgroundCopyCallback {
	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override {
		if (riid == __uuidof(IUnknown) || riid == __uuidof(IBackgroundCopyCallback)) {
			*ppvObject = this;
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	ULONG __stdcall AddRef(void) override {
		return 2;
	}
	ULONG __stdcall Release(void) override {
		return 1;
	}
	HRESULT __stdcall JobTransferred(IBackgroundCopyJob* pJob) override {
		printf("Success!\n");
		PostQuitMessage(0);
		SetEvent(g_hDone);
		return S_OK;
	}
	HRESULT __stdcall JobError(IBackgroundCopyJob* pJob, IBackgroundCopyError* pError) override {
		PWSTR text;
		auto hr = pError->GetErrorDescription(LANG_USER_DEFAULT, &text);
		if (SUCCEEDED(hr)) {
			printf("Error: %ws\n", text);
			CoTaskMemFree(text);
		}
		PostQuitMessage(0);

		SetEvent(g_hDone);

		return S_OK;
	}
	HRESULT __stdcall JobModification(IBackgroundCopyJob* pJob, DWORD dwReserved) override {
		BG_JOB_STATE state;
		BG_JOB_PROGRESS progress;
		pJob->GetState(&state);

		if (state == BG_JOB_STATE_TRANSFERRING && S_OK == pJob->GetProgress(&progress)) {
			printf("\rBytes: %llu / %llu KB",
				progress.BytesTransferred >> 10, progress.BytesTotal >> 10);
		}

		return S_OK;
	}
};

int main() {
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	CComPtr<IBackgroundCopyManager> mgr;
	auto hr = mgr.CoCreateInstance(__uuidof(BackgroundCopyManager));
	if (FAILED(hr))
		return Error(hr);

	CComPtr<IBackgroundCopyJob> job;
	GUID id;
	hr = mgr->CreateJob(L"Simple Job", BG_JOB_TYPE_DOWNLOAD, &id, &job);
	if (FAILED(hr))
		return Error(hr);

	hr = job->AddFile(L"https://link.testfile.org/500MB", L"d:\\test\\myfile.bin");
	if (FAILED(hr))
		return Error(hr);

	g_hDone = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	job->SetNotifyFlags(BG_NOTIFY_JOB_TRANSFERRED | BG_NOTIFY_JOB_MODIFICATION | BG_NOTIFY_JOB_ERROR);
	NotifyCallback cb;
	hr = job->SetNotifyInterface(&cb);
	if (FAILED(hr))
		return Error(hr);

	hr = job->Resume();
	if (FAILED(hr))
		return Error(hr);

	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
		DispatchMessage(&msg);

	//if (WAIT_TIMEOUT == WaitForSingleObject(g_hDone, 60 * 1000)) {
	//	printf("Timeout!!\n");
	//}

	return 0;
}
