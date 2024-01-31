// ShellCode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void ShellCode(PVOID) {
	auto teb = NtCurrentTeb();
	auto peb = teb->ProcessEnvironmentBlock;
	auto ldr = peb->Ldr;

	auto head = &ldr->InMemoryOrderModuleList;

	for (auto link = head->Flink; link != head; link = link->Flink) {
		auto entry = CONTAINING_RECORD(link, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		printf("%p %ws\n", entry->DllBase, entry->FullDllName.Buffer);
	}
}

#pragma code_seg("inject")

void __stdcall TestMessageBox(PCWSTR message) {
	::MessageBoxW(nullptr, message, L"Injected Code", MB_OK);
}

#pragma code_seg()

int counter;

int main() {
	HANDLE h[10];
	unsigned count = 0;
	LARGE_INTEGER tick, now, freq;
	QueryPerformanceCounter(&tick);
	QueryPerformanceFrequency(&freq);

	for(int i = 0; i < 10; ++i) {
		h[i] = ::CreateThread(nullptr, 0, [](auto p) -> DWORD {
			auto& c = *(unsigned*)p;
			for (int i = 0; i < 1000000; i++)
				InterlockedIncrement(&c);
			return 0;
			}, &count, 0, nullptr);
	}
	WaitForMultipleObjects(10, h, TRUE, INFINITE);
	QueryPerformanceCounter(&now);
	auto time = (now.QuadPart - tick.QuadPart) * 1000000 / freq.QuadPart;
	printf("Count: %d elapsed: %d usec\n", count, time);

	return 0;
}

