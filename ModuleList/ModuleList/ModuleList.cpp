// ModuleList.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <winternl.h>
#include <stdio.h>

int main() {
	auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;
	auto head = &peb->Ldr->InMemoryOrderModuleList;

	for (auto item = head->Flink; item != head; item = item->Flink) {
		auto entry = CONTAINING_RECORD(item, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
		printf("0x%p: %wZ\n", entry->DllBase, &entry->FullDllName);
	}
	return 0;
}

