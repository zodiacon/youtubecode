// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32")

int main() {
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
		return -1;

	auto sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		return -1;

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);

	if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
		return -1;

	if (listen(sock, SOMAXCONN) < 0)
		return -1;

	printf("Waiting for connections...\n");

	int len = sizeof(addr);
	auto s = accept(sock, (sockaddr*)&addr, &len);
	if (s == INVALID_SOCKET)
		return -1;

	for (;;) {
		char data[256]{};
		int size = recv(s, data, sizeof(data), 0);
		if (size < 0)
			break;

		printf("Received: %s\n", data);
		if (send(s, data, (int)strlen(data) + 1, 0) < 0)
			break;
	}
	closesocket(s);
	closesocket(sock);

	return 0;
}
