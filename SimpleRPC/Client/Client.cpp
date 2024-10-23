// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define RPC_USE_NATIVE_WCHAR
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include "..\Server\Calculator_h.h"

#pragma comment(lib, "rpcrt4")

int main() {
	RPC_WSTR binding;
	auto status = RpcStringBindingCompose(nullptr, (PWSTR)L"ncalrpc",
		nullptr, (PWSTR)L"ncalrpc:[calc]", nullptr, &binding);
	assert(status == RPC_S_OK);

	RPC_BINDING_HANDLE h;
	status = RpcBindingFromStringBinding(binding, &h);
	assert(status == RPC_S_OK);

	int result = Add(h, 3, 5);
	printf("Result: %d\n", result);

	return 0;
}

void* midl_user_allocate(size_t size) {
	return malloc(size);
}

void midl_user_free(void* p) {
	free(p);
}
