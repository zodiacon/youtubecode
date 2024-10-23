// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define RPC_USE_NATIVE_WCHAR
#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include "Calculator_h.h"

#pragma comment(lib, "rpcrt4")

int main() {
	auto status = RpcServerUseProtseqEp((PWSTR)L"ncalrpc", 1,
		(PWSTR)L"ncalrpc:[calc]", nullptr);
	assert(status == RPC_S_OK);

	status = RpcServerRegisterIf(Calculator_v0_0_s_ifspec, nullptr, nullptr);
	assert(status == RPC_S_OK);

	printf("Server PID: %u\n", GetCurrentProcessId());
	status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, FALSE);

	return 0;
}

void* midl_user_allocate(size_t size) {
	return malloc(size);
}

void midl_user_free(void* p) {
	free(p);
}

int Add(handle_t handle, int a, int b) {
	printf("Add invoked with %d and %d\n", a, b);
	return a + b;
}
