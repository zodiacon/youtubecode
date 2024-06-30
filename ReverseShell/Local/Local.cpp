// Local.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <WinSock2.h>
#include <cassert>
#include <stdio.h>
#include <ip2string.h>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "ntdll")


int main() {
	WSADATA ws;
	auto error = WSAStartup(MAKEWORD(2, 2), &ws);
	assert(error == 0);

	auto hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	assert(hSocket != INVALID_SOCKET);

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(55555);

	error = bind(hSocket, (sockaddr*)&addr, sizeof(addr));
	assert(error == 0);

	error = listen(hSocket, 1);
	assert(error == 0);

	printf("Waiting for connection...\n");
	int len = sizeof(addr);
	auto s = accept(hSocket, (sockaddr*)&addr, &len);
	assert(s != INVALID_SOCKET);

	char address[16];
	RtlIpv4AddressToStringA(&addr.sin_addr, address);

	printf("Connected to %s\n", address);

	DWORD size = 1 << 20;
	auto buffer = (char*)VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	assert(buffer);

	for (;;) {
		int bytes = recv(s, buffer, size, 0);
		if (bytes <= 0)
			break;

		buffer[bytes] = 0;
		printf("%s", buffer);

		char input[256];
		gets_s(input);
		strcat_s(input, "\n");
		send(s, input, (int)strlen(input) + 1, 0);
		if (_stricmp(input, "exit\n") == 0)
			break;
		Sleep(100);
	}

	return 0;
}
